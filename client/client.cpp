#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include <nlohmann/json.hpp>
#include <iostream>
#include <iomanip>

using json = nlohmann::json;

struct VersionData
{
    int64_t id;
    int32_t version;
    std::string url;
    std::string name;
    // 输出运算符重载
    friend std::ostream &operator<<( std::ostream &output, const VersionData &obj)
    {
        output << "id:" << obj.id << "Version:" << obj.version ;
        return output;
    }
};
// int main()
// {
//     std::cout << std::setw(4) << json::meta() << std::endl;
// }
int main(){
    std::cout << std::setw(4) << json::meta() << std::endl;

std::string response_body = "[\n"
                                "    {\n"
                                "        \"name\": \"project1\",\n"
                                "        \"extra_data\": {\n"
                                "            \"desc\": \"comment1\"\n"
                                "        },\n"
                                "        \"url\": \"https://a.b.c.0.zip\",\n"

                                "        \"id\": 100000,\n"
                                "        \"version\": 4\n"
                                "    }\n,"
                                "    {\n"
                                "        \"name\": \"projectB\",\n"
                                "        \"extra_data\": {\n"
                                "            \"desc\": \"comment2\",\n"
                                "            \"author\": \"zhangsan\"\n"
                                "        },\n"
                                "        \"url\": \"https://a.b.c/1.zip\",\n"
                                "        \"id\": 200000,\n"
                                "        \"version\": 5\n"
                                "    }"
                                "]";

nlohmann::json response_json = nlohmann::json::parse(response_body, nullptr, false);
    std::vector<VersionData> versionDataList;
    if(response_json.is_array()) {
        std::cout << "array!" << std::endl;
        for (auto p : response_json) {
            VersionData versionData;
            versionData.version = p["version"];
            versionData.id = p["id"];
            versionData.name = p["name"];
            versionData.url = p["url"];
            versionDataList.push_back(versionData);

            std::cout << "id:" << versionData.id << std::endl;
        }
        std::cout << "versionDataList:" << std::endl;
        for (std::vector<VersionData>::iterator iter = versionDataList.begin(); iter != versionDataList.end(); iter++) {
            std::cout << (*iter).id << std::endl;
            std::cout << (*iter) << std::endl;
        }
    } else {
        std::cout << "not array" << std::endl;
    }

    std::cout << "done of end!" << std::endl;


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
    
    const char* message = response_body.c_str();// "send hello from client";
    send(sock, message, strlen(message), 0);

    //接收响应
    char buffer[1024] = {0};
    recv(sock, buffer, sizeof(buffer), 0);
    std::cout << "server: "<<buffer <<std::endl;

    close(sock);
    return 0;
}