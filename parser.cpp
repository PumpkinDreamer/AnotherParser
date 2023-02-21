#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <unistd.h>

// Define the search keywords and URL
const std::string keywords = "RFP+New+jersey,construction,Renovation";
const std::string url = "https://www.google.com/search?q=" + keywords;

// Function to handle the HTTP response
size_t write_callback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

// Function to perform the web search and save the results to a file
void search_and_save() {
    // Get the current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        std::cerr << "Error getting current working directory" << std::endl;
        return;
    }

    // Set the destination file path
    std::string dest_path = std::string(cwd) + "/search_results.txt";

    // Set up the curl library
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Set up the callback function to handle the HTTP response
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Send the HTTP request and handle any errors
        CURLcode result = curl_easy_perform(curl);
        if (result != CURLE_OK) {
            std::cerr << "Error performing web search: " << curl_easy_strerror(result) << std::endl;
            return;
        }
        // Debugging: print out the HTTP response
        std::cout << "HTTP response:\n" << response << std::endl;

        // Clean up the curl library
        curl_easy_cleanup(curl);

        // Extract the search results from the response
        std::string search_results;
        size_t start_pos = response.find("<div class=\"g\">");
        size_t end_pos = response.rfind("</div>");
        if (start_pos != std::string::npos && end_pos != std::string::npos) {
            search_results = response.substr(start_pos, end_pos - start_pos);
        }

        // Debugging: print out the search results
        std::cout << "Search results:\n" << search_results << std::endl;

        // Write the search results to a file
        std::ofstream output_file(dest_path);
        if (output_file.is_open()) {
            output_file << search_results;
            output_file.close();
            std::cout << "Search results saved to " << dest_path << std::endl;
        } else {
            std::cerr << "Error opening output file" << std::endl;
        }
    }
}

int main() {
    // Print the welcome message and menu
    std::cout << "Welcome to the daily web search tool!\n\n";
    std::cout << "Select an option:\n";
    std::cout << "1. Perform a search and save the results to a file\n";
    std::cout << "2. Exit the program\n\n";

    // Process user input
    int choice;
    while (true) {
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                // Perform the search and save
                search_and_save();
                std::cout << "Search completed and results saved to file.\n\n";
                break;
            case 2:
                // Exit the program
                std::cout << "Exiting program...\n";
                return 0;
        }
    }
}