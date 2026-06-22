#include "ansi/ansi.h"
#include <cstdio>

int main() {
    ansi::print("=== Testing ansi::print ===\n");

    ansi::print("Normal text\n");
    ansi::print("Text [with red color] and [with green] back to normal\n",
                ansi::Color::LightRed,
                ansi::Color::LightGreen);

    ansi::print("Nesting: [red [green inside] back to red] normal\n",
                ansi::Color::Red,
                ansi::Color::Green);

    ansi::print("Escaping: ^[ this is not a color, just characters ^]\n");

#ifdef _WIN32
    ansi::set_console_color(ansi::Color::LightYellow);
    ansi::print("Full yellow text\n");
    ansi::reset_console_color();

    ansi::print("Code page change (Windows only): {text in CP1251} back to UTF-8\n", 1251);

#endif

    ansi::print("\n=== Testing cursor ===\n");

    const auto [x, y] = ansi::get_cursor_position();
    printf("Current cursor position: x=%d, y=%d\n", x, y);

    ansi::set_cursor_position(12, 12);
    ansi::print("Text at position 12,12\n");

    ansi::move_cursor_relative(5, 2);
    ansi::print("Relative move +5x +2y\n");

    ansi::set_cursor_position(0, 15);

    ansi::print("\n=== Testing clipboard ===\n");

#ifdef _WIN32
    if (ansi::set_clipboard_text("Text from ansi::set_clipboard_text"))
        ansi::print("Text [successfully] written to clipboard\n",
                    ansi::Color::LightGreen);

    const std::string clipboard = ansi::get_clipboard_text();
    printf("Clipboard text: %s\n", clipboard.empty() ? "(empty or error)" : clipboard.c_str());
#else
    ansi::set_clipboard_text("test");
    ansi::get_clipboard_text();
#endif

    ansi::print("\nTesting completed. Press Enter to exit...");
    std::getchar();

    return 0;
}