cmake_minimum_required(VERSION 3.25)
project(lab1cpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


option(SHARED_LIBS "Создание динамических библиотек" TRUE)
option(SEARCH_MOD "Использование встроенных контейнеров" TRUE)
option(TESTING_MOD "Компиляция тестов" TRUE)

if(SHARED_LIBS)
	set(BUILD_TYPE_LIBS SHARED)
else()
	set(BUILD_TYPE_LIBS STATIC)
endif()

if(SEARCH_MOD)
	add_compile_definitions(SEARCHMOD_UI)
endif()

add_subdirectory(tests)


include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(utilities ${BUILD_TYPE_LIBS}
	include/utilities.h
	src/utilities/utilities.cpp
)
add_library(differentMatrix ${BUILD_TYPE_LIBS}
	include/matrix.h
	include/strings.h
	src/differentMatrix/matrix.cpp
	src/differentMatrix/strings.cpp
)
add_library(wordSearch ${BUILD_TYPE_LIBS}
	include/search.h
	src/wordSearch/search.cpp
	src/wordSearch/searchMod.cpp
)
add_library(UI ${BUILD_TYPE_LIBS}
	include/UI.h
	src/UI/UI.cpp
	src/UI/UIMod.cpp
)


add_executable(lab1cpp src/main.cpp)


target_link_libraries(differentMatrix
	PRIVATE utilities
)
target_link_libraries(wordSearch
	PRIVATE differentMatrix
)
target_link_libraries(UI
	PRIVATE differentMatrix
	PRIVATE wordSearch
)
target_link_libraries(lab1cpp
	PRIVATE UI
)
