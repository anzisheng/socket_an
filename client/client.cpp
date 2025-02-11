#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main(){
    int sock;
    struct sockaddr_in server;

    //create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        std::cerr << "socket creation failed" <<std::endl;
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    //connect to server
    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        std::cerr << "connection failed" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "connected to ip : 127.0.0.1, port: 8080"<< std::endl;
    }
    
    const char* message = "send hello from client";
    send(sock, message, strlen(message), 0);

    //接收响应
    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0);
    std::cout << "server: "<<buffer <<std::endl;

    close(sock);
    return 0;
}