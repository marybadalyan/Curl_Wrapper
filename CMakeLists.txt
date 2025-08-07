cmake_minimum_required(VERSION 3.15)
project(MyCurlProject)

find_package(CURL REQUIRED)

add_executable(my_app main.cpp)

target_link_libraries(my_app PRIVATE CURL::libcurl)
