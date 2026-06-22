cmake_minimum_required(VERSION 3.31)
project(ansi-console VERSION 1.0 LANGUAGES CXX)

add_library(ansi STATIC
        src/color.cpp
        src/print.cpp
        src/read.cpp
        src/cursor.cpp
        src/clipboard.cpp
)

target_include_directories(ansi PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(ansi::ansi ALIAS ansi)

if (PROJECT_IS_TOP_LEVEL)
    add_executable(ansi-example examples/main.cpp)
    target_link_libraries(ansi-example PRIVATE ansi::ansi)
endif ()