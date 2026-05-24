cmake_minimum_required(VERSION 3.25)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts-diagnostics-depth=2")

include_directories(include)

add_library(map STATIC
    include/map.hpp
    include/field.hpp
    map.cpp
    field.cpp
)

target_link_libraries(map PUBLIC
    team
)
