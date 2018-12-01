cmake_minimum_required(VERSION 3.12)
project(GHA2018 C)

set(CMAKE_C_STANDARD 99)

find_package(Curses REQUIRED)
find_package(CURL REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${CURL_INCLUDE_DIRS})

set(SOURCE_FILES
        main.c
        menus.c
        smtp.h
        menus.h
        smtp.c accounts.h accounts.c)

add_executable(GHA2018 ${SOURCE_FILES})

target_link_libraries(GHA2018 ${CURSES_LIBRARY} menu)
target_link_libraries(GHA2018 ${CURL_LIBRARIES})