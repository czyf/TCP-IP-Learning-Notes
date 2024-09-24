#include<iostream>
#include<string>

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <unistd.h>

void error_handling(const string &message);

int main(int argc, char **argv){

    int server_sock;
    int client_sock;

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    
    socket_t client_addr_size;

    std::string message = "Hello world";

    if (argc != 2){
        std::cerr << "Usage : " << argv[0] << " <port>" << std::endl;
        exit(1);
    }
    // 调用socket函数创建套接字
    int server_socket = socket(SF_INET, SOCK_STREAM, 0);
    if (server_socket <  0){
        error_handling("socket() error");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_affr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // 调用bind函数分配ip地址和端口号
    if (-1 == (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)))){
        error_handling("bind() error");
    }
    // 调用listen函数将套接字转为可接收连接状态
    if (-1 == (listen(server_sock, 5))){
        error_handling("listen() error");
    }
    client_addr_size = sizeof(client_addr);

    // 调用accept函数受理连接请求，如果在没有连接请求的情况下调用该函数，则不会返回，直到有请求连接为止
    client_sock = accept(serv_sock, (struct sockaddr*)&client_addr, &client_addr_size);
    if(client_sock == -1){
        error_handling("accept() error");
    }

    write(client_sock, message, sizeof(message));
    close(client_sock);
    close(server_sock);
    return 0;
}

void error_handling(const string &message){
    std::cerr << message << endl;
    exit(1);
}