cmake_minimum_required(VERSION 3.15)
set(mandelbrot "mandelbrot")

project(${mandelbrot} CXX)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

set(SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mandelbrot.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_executable(${mandelbrot} ${SRCS})

target_include_directories(${mandelbrot}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    )

# Need to pull in SMFL from a different place than conan. vcpkg?
target_link_libraries(${mandelbrot} PRIVATE project_options)
