cmake_minimum_required(VERSION 3.25)
project(lab3 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fconcepts-diagnostics-depth=2")

find_package(SFML 2.5.1 COMPONENTS graphics audio window system REQUIRED)
set(SFML_STATIC_LIBRARIES TRUE)

add_executable(simulation 
    main.cpp 
    simulation.cpp 
    menu.cpp 
    texturePack.cpp 
    button.cpp 
    inputField.cpp 
    interfaceElem.cpp 
    interfaceBar.cpp 
    configs.cpp
)

target_link_libraries(simulation PUBLIC
    sfml-graphics 
    sfml-audio
    sfml-network
    sfml-window
    sfml-system
    orderedTable

    instructions
    orders
    map
    team
    soldiers
    training
)

add_custom_command(TARGET simulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/share/ $<TARGET_FILE_DIR:simulation>)

target_compile_options(simulation PRIVATE "-Wno-unused-parameter")
