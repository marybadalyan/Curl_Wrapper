#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <curl/curl.h>

// 1. RAII wrapper for curl_global_init() and curl_global_cleanup()
class CurlGlobal {
public:
    CurlGlobal() {
        // As of libcurl 7.82.0, this is no longer strictly necessary
        // but it's good practice for compatibility and thread safety.
        if (curl_global_init(CURL_GLOBAL_ALL)) {
            throw std::runtime_error("Failed to initialize libcurl globally.");
        }
    }
    ~CurlGlobal() {
        curl_global_cleanup();
    }
};

// 2. The main C++ wrapper class for a CURL easy handle
class CurlEasy {
private:
    CURL* handle;

    // This is the callback function that libcurl will call to write received data.
    // It must be a static function because C libraries cannot call C++ member functions directly.
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
        // 'userp' is the pointer we set with CURLOPT_WRITEDATA. We cast it back to our string.
        std::string* s = static_cast<std::string*>(userp);
        size_t new_length = size * nmemb;
        try {
            s->append(static_cast<char*>(contents), new_length);
        } catch (const std::bad_alloc& e) {
            // handle memory allocation failure
            return 0;
        }
        return new_length;
    }

public:
    // Constructor: Initializes the handle
    CurlEasy() : handle(curl_easy_init()) {
        if (!handle) {
            throw std::runtime_error("Failed to initialize libcurl easy handle.");
        }
    }

    // Destructor: Cleans up the handle
    ~CurlEasy() {
        if (handle) {
            curl_easy_cleanup(handle);
        }
    }

    // Forbid copying to prevent double-freeing the handle
    CurlEasy(const CurlEasy&) = delete;
    CurlEasy& operator=(const CurlEasy&) = delete;

    // A high-level function to perform a simple GET request
    std::string fetch(const std::string& url) {
        std::string response_string;

        // Set the URL
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        
        // Follow redirects
        curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

        // Set the write callback function
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_callback);

        // Set the data pointer for the callback
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &response_string);

        // Perform the request and check for errors
        CURLcode res = curl_easy_perform(handle);
        if (res != CURLE_OK) {
            throw std::runtime_error(std::string("curl_easy_perform() failed: ") + curl_easy_strerror(res));
        }

        return response_string;
    }
};

// --- Main application using the wrapper ---
int main() {
    try {
        // The CurlGlobal object will initialize/cleanup libcurl for us.
        CurlGlobal global_initializer;

        // Create an instance of our wrapper
        CurlEasy curl;

        std::cout << "Fetching https://example.com..." << std::endl;
        
        // Use the high-level fetch method. Exceptions are handled below.
        std::string content = curl.fetch("https://example.com");

        std::cout << "===== Page Content Start =====" << std::endl;
        std::cout << content.substr(0, 300) << "..." << std::endl; // Print first 300 chars
        std::cout << "===== Page Content End =====" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}