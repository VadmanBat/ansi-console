#include "ansi/ansi.h"

#include <cstdio>
#include <stack>
#include <cstdarg>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
void print(const std::string& markup_text, ...) {
    va_list args;
    va_start(args, markup_text);

    std::string buffer;
    std::stack<Color> color_stack;

#ifdef _WIN32
    const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    std::stack<WORD> attr_stack;
    CONSOLE_SCREEN_BUFFER_INFO info{};
    if (handle != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(handle, &info))
        attr_stack.push(info.wAttributes);

    std::stack<unsigned int> cp_stack;
    cp_stack.push(GetConsoleOutputCP());
#endif

    const auto last = markup_text.cend();
    for (auto it = markup_text.cbegin(); it != last; ++it)
        switch (const auto ch = *it) {
            case '[': {
                std::printf("%s", buffer.c_str());
                buffer.clear();

                int val = va_arg(args, int);
                auto c  = static_cast<Color>(val);

                color_stack.push(c);

#ifdef _WIN32
                if (handle != INVALID_HANDLE_VALUE && !attr_stack.empty()) {
                    const WORD bg_and_other = attr_stack.top() & 0xF0;
                    const WORD fg           = static_cast<WORD>(c);
                    WORD new_attrs          = bg_and_other | fg;
                    attr_stack.push(new_attrs);
                    SetConsoleTextAttribute(handle, new_attrs);
                }
#else
                std::printf("\033[%dm", get_foreground_code(c));
#endif

                break;
            }

            case ']': {
                std::printf("%s", buffer.c_str());
                buffer.clear();

#ifdef _WIN32
                if (handle != INVALID_HANDLE_VALUE && !attr_stack.empty()) {
                    attr_stack.pop();
                    if (!attr_stack.empty())
                        SetConsoleTextAttribute(handle, attr_stack.top());
                }
                if (!color_stack.empty()) {
                    color_stack.pop();
#endif
#ifndef _WIN32
                    if (!color_stack.empty())
                        std::printf("\033[%dm", get_foreground_code(color_stack.top()));
#endif
                }
                break;
            }

#ifdef _WIN32
            case '{': {
                std::printf("%s", buffer.c_str());
                buffer.clear();

                unsigned int new_cp = va_arg(args, unsigned int);
                cp_stack.push(new_cp);
                SetConsoleOutputCP(new_cp);
                break;
            }

            case '}': {
                std::printf("%s", buffer.c_str());
                buffer.clear();

                if (!cp_stack.empty()) {
                    cp_stack.pop();
                    if (!cp_stack.empty())
                        SetConsoleOutputCP(cp_stack.top());
                }
                break;
            }
#endif
            case '^':
                buffer += ++it == last ? '^' : *it;
                break;
            default:
                buffer += ch;
                break;
        }

    std::printf("%s", buffer.c_str());
    va_end(args);
}
}