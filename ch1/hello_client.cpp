#include<iostream>
#include<string>

#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(const string &message);

int main(int argc, chat** argv){
    int client_socket;
    struct sockaddr_in server_addr;

    char message[1024];
    int str_len;

    if (argc != 3){
        std::cout << "Usgae : " << argv[0] << "<IP> <port>" << endl;
        exit(1);
    }
    // 创建套接字
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1){
        error_handling(socket() error);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    // 调用connect函数向服务器发送连接请求
    if (-1 == connect(client_socket, (struct sockaddr_in*)&server_addr, sizeof(server_addr))){
        error_handling("connect() error");
    }

    str_len = read(client_socket, message, sizeof(message) - 1);
    if (str_len == -1){
        error_handling("read() error!");
    }

    std::cout << "Message from server : " << string(message) << std::endl;
    close(client_socket);
    return 0;

}

void error_handling(const string &message){
    std::cerr << message << std::endl;
    exit(1);
}