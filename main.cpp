#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

int main(){
    std::cout << "Welcome to disk report." << std::endl;
    start:
    std::cout << "What directories would you like to scan today?" << std::endl;
    std::string targetDir = "-1";
    std::cin >> targetDir;

    std::vector<std::string> directories;
    std::vector<std::string> files;

    if(fs::exists(targetDir) && targetDir != "-1") {
        for(const auto& dir : fs::recursive_directory_iterator(targetDir)){

            if (!fs::is_directory(dir)) {
                files.push_back(dir.path().string());
            }
            else if(fs::is_directory(dir)) {
            }
        }
        for (int x = 0; x < files.size() - 1; x++){
            std::cout << files[x] << std::endl;

        }
        for (int x = 0; x < directories.size() - 1; x++) {
            std::cout << directories[x] << std::endl;
        }
    }
    else{
        std::cout << "Invalid Directory!" << std::endl;
        goto start;
    }
    return 0;
}