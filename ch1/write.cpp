#include<iostream>
#include<string>

#include<cstdlib>
#include<unistd.h>
#include<fcntl.h>


void error_handling(const string &message);

int main(){
    int fd;
    char buf[] = "let`s go!";

    fd = open("data.txt", O_CREAT | O_WRONLY |O_TRUNC);
    if (fd == -1)
        error_handling("open() error!");
    std::cout << "file descriptor: " << fd << std::endl;

    if (write(fd, buf, sizeof(buf)) == -1)
        error_handling("write() error");

    close(fd);
    return 0;
}

void error_handling(const string &message){
    std::cout << message << std::endl;
    exit(1);
}