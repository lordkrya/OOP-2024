cmake_minimum_required(VERSION 3.25)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts-diagnostics-depth=2")

include_directories(include)

add_library(team STATIC
    include/battalion.hpp
    include/division.hpp
    battalion.cpp
    division.cpp
)

target_link_libraries(team PUBLIC
    orderedTable
    soldiers
    map
)
