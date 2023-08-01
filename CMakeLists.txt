cmake_minimum_required(VERSION 3.25)
project(SR1)
set(EXECUTABLE_NAME "SR1")

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
# Incluye los directorios de cabecera de GLM
include_directories("/usr/include/glm")

add_executable(${EXECUTABLE_NAME} main.cpp render.h color.h framebuffer.h vertexGML.h)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL2)

target_sources(${EXECUTABLE_NAME} PRIVATE render.cpp color.cpp framebuffer.cpp vertexGML.cpp)
