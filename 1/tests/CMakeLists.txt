# Для формирования отчёта: gcovr -r ../.. -e CMakeFiles --html-details -o coverage.html

if(TESTING_MOD)
	find_package(Catch2 REQUIRED)
	add_compile_options(-fprofile-arcs -ftest-coverage)
	link_libraries(gcov)

	add_executable(wordSearch_testing testing.cpp)
	set_target_properties(wordSearch_testing PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

	target_link_libraries(wordSearch_testing
		PRIVATE wordSearch
		PRIVATE differentMatrix
		PRIVATE utilities
		PRIVATE Catch2::Catch2WithMain
	)
endif()
