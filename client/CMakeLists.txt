cmake_minimum_required(VERSION 3.12)
project(client)

find_package(nlohmann_json 3.10.5 REQUIRED)
add_executable(client client.cpp)
target_link_libraries(client PRIVATE nlohmann_json::nlohmann_json)