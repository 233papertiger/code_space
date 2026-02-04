#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>

#define MAX_EVENTS 64
#define BUFFER_SIZE 1024
#define PORT 8080

// 创建并初始化服务器socket
int create_server_socket() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    
    // 1. 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // 2. 设置socket选项（避免端口占用错误）
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                   &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    // 3. 绑定地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // 4. 监听
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", PORT);
    return server_fd;
}

// 设置文件描述符为非阻塞模式
void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
    }
}

// LT模式服务器
void epoll_lt_server() {
    printf("Starting epoll LT server...\n");
    
    int server_fd = create_server_socket();
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    
    struct epoll_event ev, events[MAX_EVENTS];
    
    // 注册服务器socket到epoll（LT模式）
    ev.events = EPOLLIN;  // 默认是LT模式
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }
        
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // 接受新连接
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int client_fd = accept(server_fd, 
                                      (struct sockaddr *)&client_addr, 
                                      &addr_len);
                if (client_fd == -1) {
                    perror("accept");
                    continue;
                }
                
                printf("New connection from %s:%d (fd=%d)\n",
                       inet_ntoa(client_addr.sin_addr),
                       ntohs(client_addr.sin_port),
                       client_fd);
                
                // 注册客户端socket（LT模式）
                ev.events = EPOLLIN;  // LT模式
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl: client_fd");
                    close(client_fd);
                }
                
            } else {
                // 处理客户端数据
                int client_fd = events[i].data.fd;
                char buffer[BUFFER_SIZE];
                
                // LT模式：只读一次，没读完的数据下次epoll_wait还会通知
                ssize_t n = read(client_fd, buffer, sizeof(buffer));
                if (n > 0) {
                    printf("Received %ld bytes from fd %d\n", n, client_fd);
                    // Echo回数据
                    write(client_fd, buffer, n);
                } else if (n == 0) {
                    // 客户端关闭连接
                    printf("Client fd %d disconnected\n", client_fd);
                    close(client_fd);
                } else {
                    perror("read");
                    close(client_fd);
                }
            }
        }
    }
    
    close(server_fd);
    close(epoll_fd);
}

// ET模式服务器
void epoll_et_server() {
    printf("Starting epoll ET server...\n");
    
    int server_fd = create_server_socket();
    
    // 设置服务器socket为非阻塞（ET模式必须）
    set_nonblocking(server_fd);
    
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    
    struct epoll_event ev, events[MAX_EVENTS];
    
    // 注册服务器socket到epoll（ET模式）
    ev.events = EPOLLIN | EPOLLET;  // ET模式
    ev.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl: server_fd");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }
        
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_fd) {
                // ET模式：必须循环accept直到EAGAIN
                while (1) {
                    struct sockaddr_in client_addr;
                    socklen_t addr_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, 
                                          (struct sockaddr *)&client_addr, 
                                          &addr_len);
                    if (client_fd == -1) {
                        // 检查是否所有连接都已处理完
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // 这是正常情况：没有更多pending连接了
                            break;
                        } else {
                            perror("accept");
                            break;
                        }
                    }
                    
                    printf("New ET connection from %s:%d (fd=%d)\n",
                           inet_ntoa(client_addr.sin_addr),
                           ntohs(client_addr.sin_port),
                           client_fd);
                    
                    // 设置客户端socket为非阻塞（ET模式必须）
                    set_nonblocking(client_fd);
                    
                    // 注册客户端socket（ET模式）
                    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
                    ev.data.fd = client_fd;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                        perror("epoll_ctl: client_fd");
                        close(client_fd);
                    }
                }
                
            } else if (events[i].events & EPOLLIN) {
                // 处理客户端数据（ET模式必须一次性读完）
                int client_fd = events[i].data.fd;
                
                while (1) {
                    char buffer[BUFFER_SIZE];
                    ssize_t n = read(client_fd, buffer, sizeof(buffer));
                    
                    if (n > 0) {
                        printf("ET Received %ld bytes from fd %d\n", n, client_fd);
                        // Echo回数据
                        write(client_fd, buffer, n);
                        
                        // 如果读取的数据小于缓冲区，说明已经读完了
                        if (n < sizeof(buffer)) {
                            break;
                        }
                        // 否则继续读（可能有更多数据）
                        
                    } else if (n == 0) {
                        // 客户端关闭连接
                        printf("ET Client fd %d disconnected\n", client_fd);
                        close(client_fd);
                        break;
                        
                    } else if (n < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            // 数据已读完（ET模式正常退出）
                            break;
                        } else {
                            perror("read");
                            close(client_fd);
                            break;
                        }
                    }
                }
                
            } else if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                // 处理错误或断开
                printf("Error or hangup on fd %d\n", events[i].data.fd);
                close(events[i].data.fd);
            }
        }
    }
    
    close(server_fd);
    close(epoll_fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [lt|et]\n", argv[0]);
        printf("  lt - Level Triggered mode\n");
        printf("  et - Edge Triggered mode\n");
        return 1;
    }
    
    if (strcmp(argv[1], "lt") == 0) {
        epoll_lt_server();
    } else if (strcmp(argv[1], "et") == 0) {
        epoll_et_server();
    } else {
        printf("Invalid mode. Use 'lt' or 'et'\n");
        return 1;
    }
    
    return 0;
}