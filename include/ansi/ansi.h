#pragma once
// Created by Vadim 22.06.2026.

#include "ansi/constants/color-enums.hxx"

#include <string>

namespace ansi {
void print(const std::string& markup_text, ...);

std::pair<int, int> get_cursor_position();
void set_cursor_position(int x, int y);
void move_cursor_relative(int dx, int dy);

void init_colors();
int get_foreground_code(Color color);
std::string fg(Color color);
void set_console_color(Color color);
void reset_console_color();

std::string get_clipboard_text();
bool set_clipboard_text(const std::string& text);
std::string get_hidden_input();

void clear();
}