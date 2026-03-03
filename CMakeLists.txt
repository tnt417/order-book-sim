cmake_minimum_required(VERSION 3.25)
project(itch_parser)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(itch_parser
        src/itch-parser.cpp
)