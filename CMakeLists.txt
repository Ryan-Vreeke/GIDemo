cmake_minimum_required(VERSION 3.16)
project(RadianceCascade)

# Enable testing
enable_testing()

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable debug symbols
set(CMAKE_BUILD_TYPE Debug)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find external libraries using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLEW REQUIRED glew)

#find GLFW
find_package(glfw3 3.4 REQUIRED)

# Set source files for the main project
file(GLOB SOURCES src/*.cpp)
list(REMOVE_ITEM SOURCES ${CMAKE_SOURCE_DIR}/src/main.cpp)

# Add the main executable
add_executable(RadianceCascade src/main.cpp ${SOURCES})

# Link external libraries to the main executable
target_link_libraries(RadianceCascade ${GLEW_LIBRARIES})
target_link_libraries(RadianceCascade glfw)
target_include_directories(RadianceCascade PRIVATE ${CMAKE_SOURCE_DIR}/external/cyCodeBase)
