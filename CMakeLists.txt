cmake_minimum_required(VERSION 3.16)
project(libraries_frameworks LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
find_package(CURL REQUIRED)

# Task 3
add_executable(main main.cpp)
target_link_libraries(main PRIVATE CURL::libcurl)
target_compile_features(main PRIVATE cxx_std_20)