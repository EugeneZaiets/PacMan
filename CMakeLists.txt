cmake_minimum_required(VERSION 3.12.0)
project(pacman)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.h")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(PacManProject ${SOURCES} ${HEADERS})


