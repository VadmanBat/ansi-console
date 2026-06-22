#include "ansi/ansi.h"

#include <string>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif

namespace ansi {
#ifdef _WIN32
static std::wstring utf8_to_wstring(const std::string& utf8) {
    if (utf8.empty())
        return {};
    const int size = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, nullptr, 0);
    std::wstring result(size - 1, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, &result[0], size);
    return result;
}

static std::string wstring_to_utf8(const std::wstring& wstring) {
    if (wstring.empty())
        return {};
    const int size = WideCharToMultiByte(CP_UTF8, 0, wstring.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string result(size - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstring.c_str(), -1, &result[0], size, nullptr, nullptr);
    return result;
}
#endif

std::string get_clipboard_text() {
#ifndef _WIN32
    std::printf("Clipboard operations are not supported on this operating system.\n");
    return "";
#else
    if (!OpenClipboard(nullptr))
        return "";

    const HANDLE handle = GetClipboardData(CF_UNICODETEXT);
    if (handle == nullptr) {
        CloseClipboard();
        return "";
    }

    const auto* data   = static_cast<const wchar_t*>(GlobalLock(handle));
    std::string result = wstring_to_utf8(std::wstring(data));
    GlobalUnlock(handle);
    CloseClipboard();

    return result;
#endif
}

bool set_clipboard_text(const std::string& text) {
#ifndef _WIN32
    static bool warned = false;
    if (!warned) {
        std::printf("Clipboard operations are not supported on this operating system.\n");
        warned = true;
    }
    return false;
#else
    std::wstring wtext = utf8_to_wstring(text);

    if (!OpenClipboard(nullptr))
        return false;

    EmptyClipboard();

    const size_t size    = (wtext.size() + 1) * sizeof(wchar_t);
    const HGLOBAL global = GlobalAlloc(GMEM_MOVEABLE, size);
    if (global == nullptr) {
        CloseClipboard();
        return false;
    }

    auto* ptr = static_cast<wchar_t*>(GlobalLock(global));
    wcscpy(ptr, wtext.c_str());
    GlobalUnlock(global);

    SetClipboardData(CF_UNICODETEXT, global);
    CloseClipboard();

    return true;
#endif
}
}