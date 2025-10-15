cmake_minimum_required(VERSION 3.16)
project(ParticlePhysicsEngine VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3 REQUIRED COMPONENTS Graphics)

# Find all my cpp files
file(GLOB_RECURSE CORE_SOURCES src/*.cpp)

# --- The Core Library ---
# We define our core library as an INTERFACE library because it's header-only for now.
add_library(particle_physics_core ${CORE_SOURCES})

# Tell CMake that our library's public include directory is the "include" folder.
target_include_directories(particle_physics_core
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# We also specify that our library depends on SFML.
target_link_libraries(particle_physics_core
  PUBLIC
    SFML::Graphics
)

# --- Examples ---
add_executable(cpu_demo examples/cpu_demo.cpp)

# Now, we link our executable to our core library.
# By doing this, it automatically gets the include directories and SFML dependency.
target_link_libraries(cpu_demo PRIVATE particle_physics_core)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
