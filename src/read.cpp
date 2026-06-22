#include "ansi/ansi.h"

#include <iostream>
#include <windows.h>

namespace ansi {
std::string get_hidden_input() {
#ifdef _WIN32
    HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode    = 0;
    GetConsoleMode(hstdin, &mode);
    SetConsoleMode(hstdin, mode & static_cast<DWORD>(~ENABLE_ECHO_INPUT));

    std::string input;
    std::getline(std::cin, input);

    SetConsoleMode(hstdin, mode);
#else
    termios oldt{};
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::string input;
    std::getline(std::cin, input);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    printf("\n");
    return input;
}
}