#include "ansi/ansi.h"

#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
std::pair<int, int> get_cursor_position() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
        return {info.dwCursorPosition.X, info.dwCursorPosition.Y};
    return {0, 0};
#else
    std::printf("\033[6n");
    std::fflush(stdout);

    int y = 0;
    int x = 0;
    if (std::scanf("\033[%d;%dR", &y, &x) == 2) {
        return {x - 1, y - 1};
    }
    return {0, 0};
#endif
}

void set_cursor_position(const int x, const int y) {
#ifdef _WIN32
    const COORD coord{static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    if (x >= 0 && y >= 0) {
        std::printf("\033[%d;%dH", y + 1, x + 1);
    }
#endif
}

void move_cursor_relative(const int dx, const int dy) {
    const auto [x, y] = get_cursor_position();
    set_cursor_position(x + dx, y + dy);
}

void clear() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}
}