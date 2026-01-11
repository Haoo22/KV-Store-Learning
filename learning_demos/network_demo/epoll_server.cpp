#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h> 
#include <fcntl.h>     
#include <errno.h>     
#include <csignal> // ✅ 引入信号处理库

#define PORT 8080
#define MAX_EVENTS 1000 
#define BUFFER_SIZE 1024 // 恢复正常的 Buffer 大小

// 全局运行标志位 (volatile 防止编译器优化)
volatile bool stop_server = false;

// ✅ 信号处理函数：收到 Ctrl+C 时执行
void handle_signal(int sig) {
    if (sig == SIGINT) {
        std::cout << "\n[Server] Catch SIGINT (Ctrl+C), shutting down..." << std::endl;
        stop_server = true; // 修改标志位，打破死循环
    }
}

void set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main() {
    // ✅ 注册信号捕捉：当按下 Ctrl+C 时，调用 handle_signal
    signal(SIGINT, handle_signal);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) return -1;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return -1;
    }
    
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        return -1;
    }
    
    std::cout << "ET Epoll Server listening on " << PORT << " (Press Ctrl+C to stop)..." << std::endl;

    int epoll_fd = epoll_create(1);
    epoll_event event;
    event.events = EPOLLIN | EPOLLET; 
    event.data.fd = server_fd; 
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

    epoll_event events[MAX_EVENTS];

    // ✅ 循环条件改为标志位
    while (!stop_server) {
        // timeout 设为 1000ms，这样每秒都能醒来检查一次 stop_server 标志
        // 如果设为 -1，信号处理函数修改了标志位，但 epoll_wait 还在死等，就无法立即退出了
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000);

        if (n == -1) {
            // 如果是因为被信号打断的，不算出错，继续循环检查 stop_server 即可
            if (errno == EINTR) continue;
            perror("Epoll wait failed");
            break;
        }

        for (int i = 0; i < n; i++) {
            int current_fd = events[i].data.fd;

            if (current_fd == server_fd) {
                sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
                
                if (client_fd != -1) {
                    std::cout << "New Client: " << client_fd << std::endl;
                    set_nonblocking(client_fd);
                    event.events = EPOLLIN | EPOLLET; 
                    event.data.fd = client_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
                }
            }
            else if (events[i].events & EPOLLIN) {
                // ... (保持 ET 读取逻辑不变) ...
                // 这里为了代码简洁，保留简单的读取逻辑示意
                char buffer[BUFFER_SIZE];
                while (true) {
                    int valread = read(current_fd, buffer, BUFFER_SIZE - 1);
                    if (valread > 0) {
                        buffer[valread] = '\0';
                        std::cout << "[Recv " << current_fd << "]: " << buffer << std::endl;
                        // 回显
                        send(current_fd, "OK\n", 3, 0);
                    } else if (valread == 0) {
                        std::cout << "Client Disconnected: " << current_fd << std::endl;
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, current_fd, nullptr);
                        close(current_fd); // 关闭客户端连接
                        break;
                    } else {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) break;
                        close(current_fd); // 出错关闭
                        break;
                    }
                }
            }
        }
    }

    // ✅ 只有优雅退出时，才会执行到这里
    std::cout << "[Server] Cleaning up resources..." << std::endl;
    close(server_fd); // 关闭监听 socket
    close(epoll_fd);  // 关闭 epoll 实例
    std::cout << "[Server] Bye!" << std::endl;
    return 0;
}