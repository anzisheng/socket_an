#include <iostream>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <string>
#include <cstring>
int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    const int PORT = 8080;

    //step 1: create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0)
    {
        //fail
        std::cerr << "socket creation failed" <<std::endl;
        return 1;
    }

    //设置socket 选项，允许地址重连
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //设置服务器地址信息
    address.sin_family = AF_INET; //IPV4
    address.sin_addr.s_addr = INADDR_ANY; //允许接收任何IP
    address.sin_port =  htons(PORT); //设置监听的端口号

    //将socket 绑定到指定的地址和端口
    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        std::cerr << "bind failed"<<std::endl;
        return 1; 
    }

    //开始监听传入的请求连接，队列为3
    if(listen(server_fd, 3) < 0){
        std::cerr << "listen failed" << std::endl;
    }

    std::cout << "server on port:" << PORT << " , listen ..."<<std::endl;

    //死循环，接收客户端连接
    while(true)
    {
        //接收到客户端的连接请求
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if(new_socket < 0)
        {
            std::cerr << "accept failed " << std::endl;
            continue;
        }
        //接收从客户端发送的消息
        char buffer[1024] = {0};
        ssize_t bytes_read = recv(new_socket, buffer, sizeof(buffer), 0);
        if(bytes_read > 0)
        {
            std::cout << "received content is: \n" << buffer << std::endl;
            //success, and sent message to client
            const char* message = "returned hello from server";
            //step 6: send message
            send(new_socket, message, strlen(message), 0);


        }
        //close the connection with client
        close(new_socket);

    }
    //close the server socket
    close(server_fd);

    return 0;

}