cmake_minimum_required(VERSION 3.25)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts-diagnostics-depth=2")

include_directories(include)

add_library(soldiers STATIC
    include/commander.hpp
    include/fighter.hpp
    include/leader.hpp
    include/officer.hpp
    include/soldier.hpp
    include/soldierStatus.hpp
    include/specType.hpp
    include/specFactory.hpp
    commander.cpp
    fighter.cpp
    leader.cpp
    officer.cpp
    soldier.cpp
    specType.cpp
)

target_link_libraries(soldiers PUBLIC
    instructions
    orders
    team
)
