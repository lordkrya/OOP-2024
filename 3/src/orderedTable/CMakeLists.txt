cmake_minimum_required(VERSION 3.25)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts-diagnostics-depth=2")

include_directories(include)

add_library(orderedTable STATIC
    include/orderedTable.hpp
    include/OTableIterator.hpp
)
set_target_properties(orderedTable PROPERTIES LINKER_LANGUAGE CXX)
