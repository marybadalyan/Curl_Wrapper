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

# Link our executable against libcurl.
# This is the modern, preferred way to link libraries in CMake.
# The "CURL::libcurl" target is an "imported target" created by find_package(CURL).
# It automatically handles setting the include directories (-I), library paths (-L),
# and the library to link against (-lcurl).
target_link_libraries(main PRIVATE CURL::libcurl)

# That's it!