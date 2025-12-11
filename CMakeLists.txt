# CMakeLists.txt for Archer Story
cmake_minimum_required(VERSION 3.10)
project(ArcherStory VERSION 1.2 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(SOURCES
    Source.cpp
    Game.cpp
    input.cpp
    player.cpp
    arrow.cpp
    map.cpp
    monster.cpp
    monsters/ball.cpp
    monsters/flower_plant.cpp
    monsters/slime.cpp
    monsters/golem.cpp
    monsters/bat.cpp
    monsters/tree.cpp
    monsters/virus.cpp
)

# Header files (for IDE support)
set(HEADERS
    Game.hpp
    input.hpp
    pos.hpp
    sprite.hpp
    player.hpp
    arrow.hpp
    map.hpp
    monster.hpp
    monsters/ball.hpp
    monsters/flower_plant.hpp
    monsters/slime.hpp
    monsters/golem.hpp
    monsters/bat.hpp
    monsters/tree.hpp
    monsters/virus.hpp
    ExternalHeaderFiles/json.hpp
    ExternalHeaderFiles/strconv.h
)

# Create executable
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})
else()
    add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
endif()

# DxLib settings (Windows only)
if(WIN32)
    # Add DxLib include directory if needed
    # target_include_directories(${PROJECT_NAME} PRIVATE "path/to/DxLib")
    
    # Link DxLib library
    # target_link_libraries(${PROJECT_NAME} DxLib)
    
    # Set subsystem to Windows
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy data directory to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
)

# Copy config file to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/config.json ${CMAKE_BINARY_DIR}/config.json
)
