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
#include <pthread.h>

#define MAX_EVENTS 64
#define BUFFER_SIZE 1024
#define PORT 8080

// ==================== 数据结构定义 ====================

// 事件处理器类型定义
typedef void (*event_callback_t)(int fd, int events, void* arg);

// 事件处理器结构体
typedef struct event_handler {
    int fd;                      // 文件描述符
    event_callback_t read_cb;    // 读事件回调
    event_callback_t write_cb;   // 写事件回调
    void* arg;                   // 回调函数参数
    struct event_handler* next;  // 用于连接多个处理器（可选）
} event_handler_t;

// Reactor核心结构体
typedef struct reactor {
    int epoll_fd;                  // epoll文件描述符
    int running;                   // 运行标志
    pthread_t thread_id;           // Reactor线程ID
    event_handler_t* handlers;     // 事件处理器链表（可选）
    pthread_mutex_t lock;          // 线程安全锁
} reactor_t;

// ==================== Reactor核心函数 ====================

// 创建并初始化Reactor
reactor_t* reactor_create() {
    reactor_t* reactor = (reactor_t*)malloc(sizeof(reactor_t));
    if (!reactor) {
        perror("malloc reactor failed");
        return NULL;
    }
    
    // 创建epoll实例
    reactor->epoll_fd = epoll_create1(0);
    if (reactor->epoll_fd == -1) {
        perror("epoll_create1 failed");
        free(reactor);
        return NULL;
    }
    
    // 初始化其他字段
    reactor->running = 0;
    reactor->handlers = NULL;
    reactor->thread_id = 0;
    
    // 初始化互斥锁
    if (pthread_mutex_init(&reactor->lock, NULL) != 0) {
        perror("pthread_mutex_init failed");
        close(reactor->epoll_fd);
        free(reactor);
        return NULL;
    }
    
    printf("Reactor created successfully, epoll_fd=%d\n", reactor->epoll_fd);
    return reactor;
}

// 注册事件处理器到Reactor
int reactor_register(reactor_t* reactor, int fd, int events, 
                    event_callback_t callback, void* arg) {
    if (!reactor || fd < 0) {
        return -1;
    }
    
    // 创建事件处理器
    event_handler_t* handler = (event_handler_t*)malloc(sizeof(event_handler_t));
    if (!handler) {
        perror("malloc handler failed");
        return -1;
    }
    
    // 初始化处理器
    handler->fd = fd;
    handler->read_cb = callback;  // 简化：一个回调处理所有事件
    handler->write_cb = NULL;     // 如果需要可以扩展
    handler->arg = arg;
    handler->next = NULL;
    
    // 创建epoll事件
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = handler;  // 关键：将处理器指针存入epoll事件数据
    
    // 添加到epoll
    if (epoll_ctl(reactor->epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) {
        perror("epoll_ctl ADD failed");
        free(handler);
        return -1;
    }
    
    // 添加到处理器链表（线程安全）
    pthread_mutex_lock(&reactor->lock);
    handler->next = reactor->handlers;
    reactor->handlers = handler;
    pthread_mutex_unlock(&reactor->lock);
    
    printf("Registered handler for fd=%d, events=0x%x\n", fd, events);
    return 0;
}

// 注销事件处理器
int reactor_unregister(reactor_t* reactor, int fd) {
    if (!reactor) {
        return -1;
    }
    
    // 从epoll删除
    if (epoll_ctl(reactor->epoll_fd, EPOLL_CTL_DEL, fd, NULL) == -1) {
        perror("epoll_ctl DEL failed");
        return -1;
    }
    
    // 从链表中删除（线程安全）
    pthread_mutex_lock(&reactor->lock);
    event_handler_t* prev = NULL;
    event_handler_t* curr = reactor->handlers;
    
    while (curr) {
        if (curr->fd == fd) {
            if (prev) {
                prev->next = curr->next;
            } else {
                reactor->handlers = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    pthread_mutex_unlock(&reactor->lock);
    
    printf("Unregistered handler for fd=%d\n", fd);
    return 0;
}

// Reactor事件循环（线程函数）
void* reactor_event_loop(void* arg) {
    reactor_t* reactor = (reactor_t*)arg;
    struct epoll_event events[MAX_EVENTS];
    
    printf("Reactor event loop started\n");
    
    reactor->running = 1;
    while (reactor->running) {
        // 等待事件
        int nfds = epoll_wait(reactor->epoll_fd, events, MAX_EVENTS, 1000); // 1秒超时
        if (nfds == -1) {
            if (errno == EINTR) {
                continue;  // 被信号中断，继续
            }
            perror("epoll_wait failed");
            break;
        }
        
        // 处理就绪事件
        for (int i = 0; i < nfds; i++) {
            event_handler_t* handler = (event_handler_t*)events[i].data.ptr;
            if (!handler) {
                continue;
            }
            
            // 检查事件类型并调用相应的回调
            if (events[i].events & EPOLLIN) {
                if (handler->read_cb) {
                    handler->read_cb(handler->fd, events[i].events, handler->arg);
                }
            }
            
            if (events[i].events & EPOLLOUT) {
                if (handler->write_cb) {
                    handler->write_cb(handler->fd, events[i].events, handler->arg);
                }
            }
            
            // 处理错误和挂断事件
            if (events[i].events & (EPOLLERR | EPOLLHUP | EPOLLRDHUP)) {
                printf("Error or hangup on fd=%d, closing\n", handler->fd);
                reactor_unregister(reactor, handler->fd);
                close(handler->fd);
            }
        }
    }
    
    printf("Reactor event loop stopped\n");
    return NULL;
}

// 启动Reactor
int reactor_start(reactor_t* reactor) {
    if (!reactor || reactor->running) {
        return -1;
    }
    
    if (pthread_create(&reactor->thread_id, NULL, reactor_event_loop, reactor) != 0) {
        perror("pthread_create failed");
        return -1;
    }
    
    printf("Reactor started, thread_id=%lu\n", (unsigned long)reactor->thread_id);
    return 0;
}

// 停止Reactor
int reactor_stop(reactor_t* reactor) {
    if (!reactor || !reactor->running) {
        return -1;
    }
    
    reactor->running = 0;
    
    // 等待线程结束
    if (pthread_join(reactor->thread_id, NULL) != 0) {
        perror("pthread_join failed");
        return -1;
    }
    
    printf("Reactor stopped\n");
    return 0;
}

// 销毁Reactor
void reactor_destroy(reactor_t* reactor) {
    if (!reactor) {
        return;
    }
    
    // 停止Reactor
    if (reactor->running) {
        reactor_stop(reactor);
    }
    
    // 清理所有处理器
    pthread_mutex_lock(&reactor->lock);
    event_handler_t* handler = reactor->handlers;
    while (handler) {
        event_handler_t* next = handler->next;
        close(handler->fd);
        free(handler);
        handler = next;
    }
    reactor->handlers = NULL;
    pthread_mutex_unlock(&reactor->lock);
    
    // 关闭epoll
    if (reactor->epoll_fd >= 0) {
        close(reactor->epoll_fd);
    }
    
    // 销毁互斥锁
    pthread_mutex_destroy(&reactor->lock);
    
    // 释放Reactor内存
    free(reactor);
    
    printf("Reactor destroyed\n");
}

// ==================== 事件处理器回调函数 ====================

// 连接上下文结构
typedef struct {
    reactor_t* reactor;
    int fd;
    char buffer[BUFFER_SIZE];
    int buffer_len;
} connection_ctx_t;

// Accept处理器：处理新连接
void accept_handler(int fd, int events, void* arg) {
    reactor_t* reactor = (reactor_t*)arg;
    
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    // 接受新连接
    int client_fd = accept(fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd == -1) {
        perror("accept failed");
        return;
    }
    
    printf("New connection from %s:%d (fd=%d)\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port),
           client_fd);
    
    // 创建连接上下文
    connection_ctx_t* ctx = (connection_ctx_t*)malloc(sizeof(connection_ctx_t));
    if (!ctx) {
        perror("malloc connection context failed");
        close(client_fd);
        return;
    }
    
    ctx->reactor = reactor;
    ctx->fd = client_fd;
    ctx->buffer_len = 0;
    
    // 注册客户端socket到Reactor
    // 注意：这里使用EPOLLIN | EPOLLET，需要非阻塞socket
    // 为简化，这里使用LT模式
    reactor_register(reactor, client_fd, EPOLLIN, echo_handler, ctx);
}

// Echo处理器：回显数据
void echo_handler(int fd, int events, void* arg) {
    connection_ctx_t* ctx = (connection_ctx_t*)arg;
    
    if (events & EPOLLIN) {
        char buffer[BUFFER_SIZE];
        
        // 读取数据
        ssize_t n = read(fd, buffer, sizeof(buffer));
        if (n > 0) {
            printf("Received %ld bytes from fd=%d\n", n, fd);
            
            // 回显数据
            write(fd, buffer, n);
            
        } else if (n == 0) {
            // 客户端关闭连接
            printf("Client fd=%d disconnected\n", fd);
            reactor_unregister(ctx->reactor, fd);
            close(fd);
            free(ctx);
            
        } else {
            perror("read failed");
            reactor_unregister(ctx->reactor, fd);
            close(fd);
            free(ctx);
        }
    }
}

// ==================== 服务器初始化函数 ====================

// 创建服务器socket
int create_server_socket(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    
    // 创建socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }
    
    // 设置socket选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                   &opt, sizeof(opt))) {
        perror("setsockopt failed");
        close(server_fd);
        return -1;
    }
    
    // 绑定地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }
    
    // 监听
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        close(server_fd);
        return -1;
    }
    
    printf("Server socket created, fd=%d, listening on port %d\n", server_fd, port);
    return server_fd;
}

// ==================== 主函数 ====================

int main(int argc, char* argv[]) {
    printf("=== Reactor Pattern Server ===\n");
    
    // 1. 创建Reactor
    reactor_t* reactor = reactor_create();
    if (!reactor) {
        fprintf(stderr, "Failed to create reactor\n");
        return 1;
    }
    
    // 2. 创建服务器socket
    int server_fd = create_server_socket(PORT);
    if (server_fd < 0) {
        fprintf(stderr, "Failed to create server socket\n");
        reactor_destroy(reactor);
        return 1;
    }
    
    // 3. 注册服务器socket到Reactor
    if (reactor_register(reactor, server_fd, EPOLLIN, accept_handler, reactor) < 0) {
        fprintf(stderr, "Failed to register server socket\n");
        close(server_fd);
        reactor_destroy(reactor);
        return 1;
    }
    
    // 4. 启动Reactor
    if (reactor_start(reactor) < 0) {
        fprintf(stderr, "Failed to start reactor\n");
        close(server_fd);
        reactor_destroy(reactor);
        return 1;
    }
    
    // 5. 主线程等待用户输入退出
    printf("\nServer is running. Press 'q' + Enter to quit.\n");
    
    char cmd;
    while (1) {
        cmd = getchar();
        if (cmd == 'q' || cmd == 'Q') {
            break;
        }
    }
    
    // 6. 清理资源
    printf("\nShutting down server...\n");
    
    // 注销服务器socket
    reactor_unregister(reactor, server_fd);
    close(server_fd);
    
    // 停止并销毁Reactor
    reactor_stop(reactor);
    reactor_destroy(reactor);
    
    printf("Server shutdown complete.\n");
    return 0;
}