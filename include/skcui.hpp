#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <limits>
#include <optional>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define KEY_UP    '\x01'
#define KEY_DOWN  '\x02'
#define KEY_ENTER '\n'

inline char getKey()
{
#ifdef _WIN32
    int key = _getch();

    if (key == '\r')
        return KEY_ENTER;

    if (key == 0 || key == 224)
    {
        key = _getch();

        if (key == 72) return KEY_UP;
        if (key == 80) return KEY_DOWN;
    }

    return key;

#elif defined(__linux__) || defined(__APPLE__)
    termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char key = getchar();

    if (key == '\n')
        key = KEY_ENTER;

    if (key == '\033') {
        getchar(); // '['
        key = getchar();

        if (key == 'A') key = KEY_UP;
        if (key == 'B') key = KEY_DOWN;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return key;

#else
    return '\0';
#endif
}

namespace skcui {


	inline void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
	}

	inline void menu(int& selected, const std::vector<std::string>& options, const std::optional<std::string_view> title = std::nullopt, const std::optional<std::string_view> desc = std::nullopt) {
		selected = 0;
		bool running = true;

        loop:		
            
            clearScreen();            
            
            if (title) {
                std::cout << *title << std::endl << std::endl;

                if (desc) {
                    std::cout << *desc << std::endl << std::endl;
                }
            }
            for (int x = 0; x < options.size(); x++) {
                if (selected == x) {
                    std::cout << "> ";
                }

                std::cout << options[x] << std::endl;
            }
            const char key = getKey();
            if (key == KEY_UP && selected > 0) {
                selected--;
            }
            else if (key == KEY_DOWN && selected < options.size() - 1) {
                selected++;
            }
            else if (key == KEY_ENTER) {
                running = false;
            }
            if (running == true) {
                goto loop;
            }
	}
}
