## Documentation

### TamperProtect

This C++ code is a simple program designed to compute the SHA256 hash of its own executable file and compare it against a known good hash. This is often used as a tamper detection mechanism to ensure that the executable has not been altered in any way. Below, I'll break down the code step by step.

### Code Breakdown

1. **Includes and Namespace**:
    ```cpp
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
    ```
   - The code includes necessary headers for input/output operations, file handling, string manipulation, hashing with OpenSSL, exception handling, and filesystem operations. 
   - The `filesystem` library is aliased as `fs` for convenience.

2. **SHA256 Calculation Function**:
    ```cpp
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
    ```
   - This function reads a file specified by `file_path` in binary mode and computes its SHA256 hash.
   - It initializes the SHA256 context, reads the file in chunks (to handle large files), and updates the hash accordingly.
   - After reading the file, it finalizes the hash and converts the raw hash bytes into a hexadecimal string.

3. **Main Function**:
    ```cpp
    int main(int argc, char* argv[]) {
        const std::string known_good_hash = "your_known_good_hash_here"; // Replace with your known good hash

        std::string executable_path = fs::absolute(argv[0]).string(); // Get the absolute path of the current executable

        try {
            std::string current_hash = calculate_sha256(executable_path);

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
    ```
   - The main function sets a placeholder for the known good hash, which should be replaced with the actual SHA256 hash of the untouched executable.
   - It retrieves the absolute path of the currently running executable using `argv[0]`, which contains the path of the program.
   - It calls the `calculate_sha256` function to compute the current hash of the executable.
   - Finally, it compares the computed hash with the known good hash and outputs whether tampering has been detected or not.
   - Any exceptions thrown during the hash calculation (such as file opening errors) are caught and printed.

### Key Concepts
- **SHA256 Hashing**: A cryptographic hash function that produces a fixed-size (256-bit) hash value from variable-sized input data.
- **File I/O**: Reading the executable file in binary mode to ensure accurate hashing.
- **Exception Handling**: Using try-catch blocks to handle potential errors in file operations.
- **Filesystem Operations**: Using the filesystem library to work with file paths, making the program more robust and portable.

### Usage
To use this program, you would compile it, run it, and provide the known good hash of the executable. The program then checks if its own binary has been altered by comparing its computed hash against the known good hash value. If they match, the executable is considered unaltered; if not, it signals potential tampering.


