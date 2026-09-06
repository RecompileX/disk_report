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
    std::string dir = "-1";
    std::cin >> dir;

    if(fs::exists(dir) && dir != "-1"){
        std::cout << "Dir is real." << std::endl;
    }
    else{
        std::cout << "Invalid Directory!" << std::endl;
        goto start;
    }
    return 0;
}