#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <openssl/sha.h>
#include <stdexcept>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

// Function to compute the SHA256 hash of a file
std::string calculate_sha256(const std::string &file_path) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        SHA256_Update(&sha256, buffer, file.gcount());
    }
    // Handle the last chunk
    if (file.gcount() > 0) {
        SHA256_Update(&sha256, buffer, file.gcount());
    }
    SHA256_Final(hash, &sha256);

    std::ostringstream oss;
    for (const auto &byte : hash) {
        oss << std::setw(2) << std::setfill('0') << std::hex << static_cast<unsigned int>(byte);
    }
    return oss.str();
}

int main(int argc, char* argv[]) {
    // Known good hash (replace with the actual hash of your compiled binary)
    const std::string known_good_hash = "your_known_good_hash_here"; // Replace with your known good hash

    // Get the path of the executable
    std::string executable_path = fs::absolute(argv[0]).string(); // Get the absolute path of the current executable

    try {
        // Calculate the current hash of the executable
        std::string current_hash = calculate_sha256(executable_path);

        // Compare with the known good hash
        if (current_hash == known_good_hash) {
            std::cout << "No tampering detected." << std::endl;
        } else {
            std::cout << "Tampering detected!" << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

