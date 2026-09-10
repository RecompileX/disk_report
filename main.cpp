#define BTOGB 1073741824
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <iostream>

namespace fs = std::filesystem;

enum state {
    START,
    SPACE,
    EXTENSIONS,
    AVAILABLE,
    DIRECTORIES_SPACE,
    TOP3
};

int main(){
    std::cout << "Welcome to disk report." << std::endl;

    start:
    state programState = START;
    std::cout << "What directories would you like to scan today?" << std::endl;
    std::string targetDir = "-1";
    std::cin >> targetDir;

    bool fileExists = false;
    std::vector<std::string> directories;
    std::vector<std::string> files;
    std::vector<std::string> fileExtension;
    std::vector<int> fileExtensionAmount;

    if(fs::exists(targetDir) && targetDir != "-1") {
        for(const auto& dir : fs::recursive_directory_iterator(targetDir)){
            if (!fs::is_directory(dir)) {
                files.push_back(dir.path().string());
            }
                else if(fs::is_directory(dir) && !fs::is_empty(dir.path())) {
                directories.push_back(dir.path().string());
            }
        }
        for (int x = 0; x < files.size(); x++){
            std::string fileExtensionsTMP;
            fileExtensionsTMP = fs::path(files[x]).extension().string();
            for (int a = 0; a < fileExtension.size(); a++) {
                fileExists = false;
                if (fileExtension[a] == fileExtensionsTMP) {
                    fileExists = true;
                    fileExtensionAmount[a]++;
                    break;
                }
            }
            if (fileExists == false) {
                fileExtension.push_back(fileExtensionsTMP);
                fileExtensionAmount.push_back(1);
            }
        }
        auto directoriesCopy = std::move(directories);
        directories.clear();
        for (int x = 0; x < directoriesCopy.size(); x++) {
            fs::path p = directoriesCopy[x];
            p.replace_extension();
            std::string directoriesTMP = p.string();
            fileExists = false;
            for (int a = 0; a < directories.size(); a++) {
                fileExists = false;
                if (directories[a] == directoriesTMP) {
                    fileExists = true;
                    break;
                }
            }

            if (fileExists == false) {
                directories.push_back(directoriesTMP);
            }
        }
        std::cout << "Directories:" << std::endl;
        for (int x = 0; x < directories.size(); x++) {
            std::cout << directories[x] << std::endl;
        }
        std::cout << "End Directories" << std::endl << std::endl;

        std::cout << "File Extensions:" << std::endl;
        for (int x = 0; x < fileExtension.size(); x++) {
            std::cout << fileExtension[x] << ' ' << fileExtensionAmount[x] << std::endl;
        }
        std::cout << "End File Extensions" << std::endl;
    }
    else{
        std::cout << "Invalid Directory!" << std::endl;
        goto start;
    }
    return 0;
}