cmake_minimum_required(VERSION 3.25)
project(lab1cpp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories(${CMAKE_SOURCE_DIR}/include)


option(SHARED_LIBS "Создание динамических библиотек" TRUE)
option(TESTING_MOD "Компиляция тестов" TRUE)

if(SHARED_LIBS)
	set(BUILD_TYPE_LIBS SHARED)
else()
	set(BUILD_TYPE_LIBS STATIC)
endif()

if(TESTING_MOD)
    add_subdirectory(tests)
endif()


add_library(utilities ${BUILD_TYPE_LIBS}
	include/utilities/utilities.h
	src/utilities/utilities.cpp
)

add_library(signals ${BUILD_TYPE_LIBS}
	include/signals/binarySignal.h
    include/signals/signalStatus.h
	src/signals/binarySignal.cpp
	src/signals/signalStatus.cpp
)

target_link_libraries(signals
	PRIVATE utilities
)
