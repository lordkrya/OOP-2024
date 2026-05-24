cmake_minimum_required(VERSION 3.25)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts-diagnostics-depth=2")

include_directories(include)

add_library(orders STATIC
    include/baseOrder.hpp
    include/orders.hpp
    orders.cpp
    include/orderFactory.hpp
    include/wait.hpp
    include/fire.hpp
)
