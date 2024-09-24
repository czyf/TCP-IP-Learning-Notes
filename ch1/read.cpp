#include<iostream>
#include<string>

#include<cstdlib>
#include<unistd.h>
#include<fcntl.h>

const int BUF_SIZE 100;

void error_handling(const string &message);

int main(){
    int fd;
    char buf[BUF_SIZE];

    fd = open("data.txt", O_RDONLY);
    if (fd == -1)
        error_handling("open() error!");
    std::cout << "file descriptor: " << fd << std::endl;

    if (read(fd, buf, sizeof(buf)) == -1)
        error_handling("read() error");
    std::cout << "file data: " << buf << std::endl;
    close(fd);
    return 0;
}

void error_handling(const string &message){
    std::cout << message << std::endl;
    exit(1);
}