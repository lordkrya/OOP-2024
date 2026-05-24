cmake_minimum_required(VERSION 3.25)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts-diagnostics-depth=2")

# Для формирования отчёта: gcovr -r ../.. -e CMakeFiles --html-details -o coverage.html
find_package(Catch2 REQUIRED PATHS "../../../lib/Catch2")
add_compile_options(-fprofile-arcs -ftest-coverage)
link_libraries(gcov)

add_executable(OTableTest OTableTest.cpp)

target_link_libraries(OTableTest
    PRIVATE orderedTable
    PRIVATE Catch2::Catch2WithMain
)
