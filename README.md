# C++ RAII Wrapper for libcurl

A simple, modern C++ wrapper for the libcurl easy interface. This project demonstrates how to create a robust, exception-safe wrapper using RAII (Resource Acquisition Is Initialization) principles.

The primary goal is to provide a clean, high-level abstraction for making HTTP requests without manually managing libcurl handles and global state.

---

## Features

- **RAII for Global State**: `CurlGlobal` class automatically calls `curl_global_init()` on creation and `curl_global_cleanup()` on destruction, ensuring the libcurl environment is managed correctly and safely.
- **RAII for Easy Handles**: `CurlEasy` class wraps a `CURL*` handle, ensuring `curl_easy_cleanup()` is called when the object goes out of scope. Copying is disabled to prevent double-free errors.
- **Simple GET Requests**: Includes a high-level `fetch(url)` method that performs a GET request and returns the response body as a `std::string`.
- **Exception-Based Error Handling**: Converts libcurl error codes into C++ exceptions for cleaner error handling.
- **Cross-Platform Build**: Uses CMake for building and includes a GitHub Actions workflow to verify the build on Linux, macOS, and Windows.

---

## Prerequisites

To build and run this project, you will need:

- A C++11 compliant compiler (e.g., GCC, Clang, MSVC)
- [CMake](https://cmake.org/) (version 3.15 or newer)
- libcurl development libraries

---

## Building the Project

### Clone the repository:

```bash
git clone https://github.com/<YOUR_USERNAME>/<YOUR_REPOSITORY>.git
cd <YOUR_REPOSITORY>
````

### Create a build directory:

```bash
mkdir build
cd build
```

### Configure with CMake:

#### On Linux/macOS:

```bash
cmake ..
```

#### On Windows (with vcpkg):

Make sure you have installed curl via vcpkg:

```bash
vcpkg install curl:x64-windows
```

Then run CMake with the vcpkg toolchain file:

```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

### Build the executable:

```bash
cmake --build .
```

---

## Usage

The `main.cpp` file provides a simple example of how to use the wrapper to fetch the content of a webpage.

```cpp
#include "CurlEasy.h" // Assuming you've separated the classes into headers
#include <iostream>

int main() {
    try {
        // The CurlGlobal object will initialize/cleanup libcurl for us.
        CurlGlobal global_initializer;

        // Create an instance of our wrapper
        CurlEasy curl;

        std::cout << "Fetching https://example.com..." << std::endl;
        
        // Use the high-level fetch method.
        std::string content = curl.fetch("https://example.com");

        std::cout << "===== Page Content Start =====" << std::endl;
        std::cout << content.substr(0, 300) << "..." << std::endl;
        std::cout << "===== Page Content End =====" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

### Run the application:

After building, run the application from the build directory:

```bash
./my_app
```

---

## Continuous Integration

This project is configured with a GitHub Actions workflow defined in `.github/workflows/cmake-multi-platform.yml`. This CI pipeline automatically builds and tests the project on:

* **Ubuntu** (with GCC and Clang)
* **macOS** (with Clang)
* **Windows** (with MSVC)

This ensures that the code remains portable and compiles correctly across major operating systems.

---
