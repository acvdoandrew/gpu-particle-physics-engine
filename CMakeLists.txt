cmake_minimum_required(VERSION 3.16)
project(ParticlePhysicsEngine VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find SMFL
find_package(SFML 3 REQUIRED COMPONENTS Graphics)

# Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# # Source files
# file(GLOB_RECURSE SOURCES
#     ${PROJECT_SOURCE_DIR}/src/*.cpp
# )
#
# # Create library
# add_library(particle_physics_core ${SOURCES})
# # Link SMFL to our core library as a PUBLIC dependency
# target_link_libraries(particle_physics_core
#   PUBLIC
#     SFML::Graphics
#     SFML::Audio
#     SFML::Network
# )

# Examples
add_executable(stage_01 examples/stage_01.cpp)
target_link_libraries(stage_01 PRIVATE SFML::Graphics)
