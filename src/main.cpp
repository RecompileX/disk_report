#define btogib 1073741824.0f
#define btogb 1000000000.0f
#define btomb 1000000.0f
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <iomanip>
#include <memory>
#include "skcui.hpp"

namespace fs = std::filesystem;

enum state {
    START,
    SPACE,
    EXTENSIONS,
    AVAILABLE,
    DIRECTORIES_SPACE,
    TOP3F
};

int main()
{
    const std::string title = "Welcome to disk report.";
    std::cout << title << std::endl;
    std::vector<std::string> entries = {
        "Space on Disk",
        "Extensions Count",
        "Available Space",
        "Full Directories Space",
        "Top 3 Files"

    };

    std::vector<std::string> entriesType{"GiB","GB","MB"};

    std::vector<std::pair<std::string, int>> fileSizeName;

    start:
    state programState = START;
    std::string targetDir;
    int selected = 0;

    skcui::menu(selected, entries, title);
    skcui::clearScreen();

    int selectedType = 0;

    float type = btogb;

    if (selectedType == 0)
        type = btogib;
    else if (selectedType == 1)
        type = btogb;
    else if (selectedType == 2)
        type = btomb;

    std::cout << "What directories would you like to scan today?" << std::endl << std::endl;
    std::cout << "Currently selected option: " << entries[selected] << std::endl << std::endl;
    std::cout << "Enter 'exit' or 'back' to return to main menu." << std::endl;
    std::getline(std::cin, targetDir);

    if (targetDir == "exit") return 0;
    if (targetDir == "back") goto start;
    if (!fs::exists(targetDir) || targetDir == "-1"){
        std::cout << "Invalid Directory! Press any key to continue..." << std::endl;
        std::string dummy;
        std::getline(std::cin, dummy);
        goto start;
    }

    switch (selected)
    {
        case 0: programState = SPACE; break;
        case 1: programState = EXTENSIONS; break;
        case 2: programState = AVAILABLE; break;
        case 3: programState = DIRECTORIES_SPACE; break;
        case 4: programState = TOP3F;
    }
    if (programState != EXTENSIONS )
        skcui::menu(selectedType, entriesType, "Which type of measurement would you like to be displayed?");
    skcui::clearScreen();

    bool fileExists = false;
    std::vector<std::string> directories;
    std::vector<std::string> files;
    std::vector<std::string> fileExtension;
    std::vector<int> fileExtensionAmount;
    std::vector<std::pair<std::string,int>> fileStorage;
    float directorySize = 0.0f;

    if(fs::exists(targetDir) && targetDir != "-1") {
        for(const auto& dir : fs::recursive_directory_iterator(targetDir)){
            if (!fs::is_directory(dir)) {
                files.push_back(dir.path().string());
                fileStorage.push_back(std::make_pair(dir.path().string(),fs::file_size(dir)));
            }
            else if(fs::is_directory(dir) && !fs::is_empty(dir.path())) {
                    directories.push_back(dir.path().string());
            }
            if (std::filesystem::is_regular_file(dir) && programState == DIRECTORIES_SPACE) {
                    float fileSize = fs::file_size(dir);
                    directorySize += fileSize / type;
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
        switch (programState){
            case START:
                std::cout << "Error program state is invalid." << std::endl;
            break;

            case DIRECTORIES_SPACE:
                std::cout << "Directory Size: " << std::fixed << std::setprecision(2) << directorySize << " " << entriesType[selectedType] << std::endl;
            break;

            case EXTENSIONS:
                std::cout << "File Extensions:" << std::endl;
                for (int x = 0; x < fileExtension.size(); x++) {
                    std::cout << fileExtension[x] << ' ' << fileExtensionAmount[x] << std::endl;
                }
            break;

            case AVAILABLE:
                std::cout << "Available space: " << std::fixed << std::setprecision(2) << fs::space_info().free / type << " "  << entriesType[selectedType];
            break;

            case SPACE:
                std::cout << "Available space: " << std::fixed << std::setprecision(2) << fs::space_info().capacity / type << " "  << entriesType[selectedType];
            break;

            case TOP3F:
                std::cout << "Top 3 files and their names:" << std::endl;
                std::sort(fileSizeName.begin(), fileSizeName.end(),
                    [](const auto& a, const auto& b) {return a.second > b.second;});
                fileSizeName.resize(3);
                for (int x = 0; x < fileSizeName.size(); x++)
                {
                    std::cout << fileSizeName[x].first << " ";
                    std::cout << fileSizeName[x].second / type << " " << entriesType[selectedType] << std::endl;

                }
        }
    }
    std::cout << std::endl <<"Press any key to continue..." << std::endl;
    std::string dummy;
    std::getline(std::cin, dummy);
    return 0;

}