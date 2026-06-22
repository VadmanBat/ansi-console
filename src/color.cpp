#include "ansi/ansi.h"

#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
int get_foreground_code(Color color) {
    static constexpr int base_codes[8] = {
        30, // Black
        34, // Blue
        32, // Green
        36, // Cyan
        31, // Red
        35, // Magenta/Violet
        33, // Yellow
        37  // White
    };

    const int val  = static_cast<int>(color);
    const int code = base_codes[val & 7];
    return val >= 8 ? code + 60 : code;
}

std::string fg(Color color) {
    return "\033[" + std::to_string(static_cast<int>(color)) + 'm';
}

void init_colors() {
#ifdef _WIN32
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode;
    if (GetConsoleMode(handle, &dwMode)) {
        dwMode |= 0x0004;
        SetConsoleMode(handle, dwMode);
    }
#endif
}

void set_console_color(Color color) {
#ifdef _WIN32
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle != INVALID_HANDLE_VALUE) {
        CONSOLE_SCREEN_BUFFER_INFO info{};
        if (GetConsoleScreenBufferInfo(handle, &info)) {
            const WORD new_attrs = (info.wAttributes & 0xF0) | static_cast<WORD>(color);
            SetConsoleTextAttribute(handle, new_attrs);
        }
    }
#else
    std::printf("\033[%dm", get_foreground_code(color));
#endif
}

void reset_console_color() {
#ifdef _WIN32
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle != INVALID_HANDLE_VALUE)
        SetConsoleTextAttribute(handle, 7);
#else
    std::printf("\033[0m");
#endif
}
}