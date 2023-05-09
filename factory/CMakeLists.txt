cmake_minimum_required(VERSION 3.20)

project(Factory LANGUAGES CXX)

set(SRCS
   src/truck.cpp
   src/boat.cpp
)

add_executable("factory" src/main.cpp ${SRCS})

target_include_directories("factory"
   PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
