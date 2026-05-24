# Для формирования отчёта: gcovr -r ../.. -e CMakeFiles --html-details -o coverage.html

find_package(Catch2 REQUIRED PATHS "../../../lib/Catch2")
add_compile_options(-fprofile-arcs -ftest-coverage)
link_libraries(gcov)

add_executable(signals_testing testing.cpp)
set_target_properties(signals_testing PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_link_libraries(signals_testing
	PRIVATE signals
	PRIVATE Catch2::Catch2WithMain
)
