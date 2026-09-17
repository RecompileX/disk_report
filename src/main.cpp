#define btogib 1073741824
#define btogb 1000000000
#define btomb 1000000
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <cstdint>
#include <memory>
#define FTXUI_IMPLEMENTATION
#include "ftxui_all.hpp"

namespace fs = std::filesystem;
using namespace ftxui;

enum state {
    START,
    SPACE,
    EXTENSIONS,
    AVAILABLE,
    DIRECTORIES_SPACE,
    TOP3D,
    TOP3F
};

int main(){
    std::cout << "Welcome to disk report." << std::endl;
    std::vector<std::string> entries = {
      "SPACE",
      "EXTENSIONS",
      "AVAILABLE",
      "DIRECTORIES_SPACE",
      "TOP3D",
      "TOP3F"
    };  

    start:
std::string targetDir;
int selected = 0;
auto screen = ScreenInteractive::TerminalOutput();
auto menu = Menu(&entries, &selected);
auto input = Input(&targetDir, "Directory");

auto component = Container::Vertical({
    input,
    menu
    }) | border;

auto app = App::TerminalOutput();

component = component | CatchEvent([&](Event event) {
    if (event == Event::Return) {
        screen.ExitLoopClosure()();
        return true;
    }
    return false;
    });

screen.Loop(component);
system("cls");


    app.Loop(component);
    state programState = DIRECTORIES_SPACE;
    std::cout << "What directories would you like to scan today?" << std::endl;

    bool fileExists = false;
    std::vector<std::string> directories;
    std::vector<std::string> files;
    std::vector<std::string> fileExtension;
    std::vector<int> fileExtensionAmount;
    std::vector<std::pair<std::string,int>> fileStorage;
    float directorySize = 0;
    float fileSize = 0;

    if(fs::exists(targetDir) && targetDir != "-1") {
        for(const auto& dir : fs::recursive_directory_iterator(targetDir)){
            if (!fs::is_directory(dir)) {
                files.push_back(dir.path().string());
                fileStorage.push_back(std::make_pair(dir.path().string(),fs::file_size(dir)));
            }
            else if(fs::is_directory(dir) && !fs::is_empty(dir.path())) {
                directories.push_back(dir.path().string());
            }
            if (targetDir.rfind("--dirsp")) {
                if (std::filesystem::is_regular_file(dir) && programState == DIRECTORIES_SPACE) {
                    float fileSize = fs::file_size(dir);
                    directorySize += fileSize / btogib;
                }
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
        std::cout << "End File Extensions" << std::endl << std::endl;
        switch (programState) {

            case START:
                std::cout << "Error program state is invalid." << std::endl;
                break;

            case DIRECTORIES_SPACE:
            std::cout << "Directory Size: " << std::fixed << std::setprecision(2) << directorySize << " Gib" << std::endl;

            default:
                break;
        }
    }
    else{
        std::cout << "Invalid Directory!" << std::endl;
        goto start;
    }
    return 0;
}