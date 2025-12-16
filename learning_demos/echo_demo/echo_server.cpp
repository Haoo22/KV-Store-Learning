#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    // 1. 创建 Socket (插座)
    // AF_INET: IPv4
    // SOCK_STREAM: TCP 流式传输
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket create failed" << std::endl;
        return -1;
    }

    // 2. 配置地址和端口
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // 监听本机所有 IP
    address.sin_port = htons(PORT);       // 端口号转为网络字节序 (Host to Network Short)

    // 端口复用 (防止重启时报 Address already in use)
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. 绑定 (Bind)
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // 4. 监听 (Listen)
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    // 5. 接受连接 (Accept) - 阻塞等待客人进门
    // 注意：accept 返回的是一个新的 socket，专门用于和这个客户端通信
    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }

    std::cout << "Connection established!" << std::endl;

    // 6. 简单的 Echo 逻辑 (收什么发什么)
    char buffer[BUFFER_SIZE] = {0};
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        // 读数据
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        
        // 如果读到 0，说明客户端断开了连接
        if (valread <= 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }
        
        std::cout << "Received: " << buffer << std::endl;

        // 发回去
        const char* hello = "Server received: ";
        send(new_socket, hello, strlen(hello), 0);
        send(new_socket, buffer, strlen(buffer), 0);
    }

    // 7. 关闭
    close(new_socket);
    close(server_fd);
    return 0;
}

// #### ✅ 任务三：编译与运行

// 这次我们不用 CMake，直接用 `g++` 编译，体验一下原生快感。

// ```bash
// # 1. 编译
// g++ simple_server.cpp -o server

// # 2. 运行 (Server 启动)
// ./server
// ```

// #### ✅ 任务四：测试连接

// Server 启动后会卡在 `listening...` 那里不动（这是正常的，它在等连接）。
// 请**打开一个新的终端窗口**（在 VS Code 里点右下角的 `+`），扮演客户端。

// 输入：
// ```bash
// telnet 127.0.0.1 8080