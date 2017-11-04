# Based on examples from https://github.com/ruslo/hunter/tree/master/examples

cmake_minimum_required(VERSION 3.8)

# Set HUNTER_ROOT environment variable to local directory if not set already
string(COMPARE EQUAL "$ENV{HUNTER_ROOT}" "" hunter_root_is_empty)
if(hunter_root_is_empty)
    get_filename_component(HUNTER_ROOT
        "${CMAKE_CURRENT_LIST_DIR}/hunter_installs"
        ABSOLUTE
    )
endif()

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.104.tar.gz"
    SHA1 "34a6ae7bc9d6a6599d21c239a60ebc05abe8abb4"
)

project(Raytracer)

add_definitions(-Wno-deprecated-declarations -DCMAKE_BUILD_TYPE=Release -DGLM_FORCE_RADIANS)

#Set the correct output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

## Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11 -pthread")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")  ## Strip binary
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Download dependencies
hunter_add_package(glm)
hunter_add_package(Boost COMPONENTS system filesystem)

# Find dependencies
find_package(glm REQUIRED)
find_package(Boost CONFIG REQUIRED system filesystem)

# Add executables
add_executable(raytracer
    src/main.cpp
    src/objects/Object3D.hpp
    src/objects/Object3D.cpp
    src/objects/Triangle.hpp
    src/objects/Triangle.cpp
    src/objects/ObjModel.hpp
    src/objects/ObjModel.cpp
)

# Link libraries
target_link_libraries(raytracer glm)
target_link_libraries(raytracer Boost::system Boost::filesystem)
