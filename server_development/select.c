#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fds[MAX_CLIENTS];
    struct sockaddr_in addr;
    int addr_len = sizeof(addr);
    fd_set readfds;
    int max_fd;
    
    // 初始化client_fds
    for (int i = 0; i < MAX_CLIENTS; i++) client_fds[i] = 0;
    
    // 创建socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // 绑定
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr*)&addr, sizeof(addr));
    
    // 监听
    listen(server_fd, 5);
    
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        max_fd = server_fd;
        
        // 添加所有客户端socket到fd_set
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] > 0) {
                FD_SET(client_fds[i], &readfds);
                if (client_fds[i] > max_fd) max_fd = client_fds[i];
            }
        }
        
        // 等待事件
        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        
        // 新连接
        if (FD_ISSET(server_fd, &readfds)) {
            int new_client = accept(server_fd, (struct sockaddr*)&addr, (socklen_t*)&addr_len);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_fds[i] == 0) {
                    client_fds[i] = new_client;
                    break;
                }
            }
        }
        
        // 处理客户端数据
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_fds[i] > 0 && FD_ISSET(client_fds[i], &readfds)) {
                char buffer[BUFFER_SIZE];
                int valread = read(client_fds[i], buffer, BUFFER_SIZE);
                if (valread == 0) {
                    // 客户端断开
                    close(client_fds[i]);
                    client_fds[i] = 0;
                } else {
                    // 处理数据
                    write(client_fds[i], buffer, valread);
                }
            }
        }
    }
    
    return 0;
}