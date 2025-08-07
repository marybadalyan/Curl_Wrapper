# Specify the minimum version of CMake required to build this project.
cmake_minimum_required(VERSION 3.10)

# Define the project name and specify that it's a C++ project.
project(CurlWrapperExample CXX)

# Set the C++ standard to C++17. The code works with C++11, but 17 is a good modern choice.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Finding and Linking libcurl ---

# This is the core command. It tells CMake to find the libcurl library.
# CMake has a built-in "FindCURL.cmake" module that knows how to search for it.
# The "REQUIRED" keyword means CMake will stop with an error if it can't find libcurl.
find_package(CURL REQUIRED)

# Define the executable target.
# This creates an executable named "curl_wrapper" from the "curl_wrapper.cpp" source file.
add_executable(main main.cpp)



# That's it!