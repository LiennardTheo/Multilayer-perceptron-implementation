cmake_minimum_required(VERSION 3.16)
project(myTorch)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g3)

set(
        SOURCES
        src/main.cpp
        src/neuralNetwork.cpp
        src/parser.cpp
)

add_executable(myTorch ${SOURCES})
target_include_directories(myTorch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)