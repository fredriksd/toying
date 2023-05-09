cmake_minimum_required(VERSION 3.20)

project(monte-carlo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(warnings
   -Wall
   -Wextra # reasonable and standard
   -Wextra-semi # Warn about semicolon after in-class function definition.
   -Wshadow # warn the user if a variable declaration shadows one from a parent context
   -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps
   # catch hard to track down memory errors
   -Wold-style-cast # warn for c-style casts
   -Wcast-align # warn for potential performance problem casts
   -Wunused # warn on anything being unused
   -Woverloaded-virtual # warn if you overload (not override) a virtual function
   -Wpedantic # warn if non-standard C++ is used
   -Wconversion # warn on type conversions that may lose data
   -Wsign-conversion # warn on sign conversions
   -Wnull-dereference # warn if a null dereference is detected
   -Wdouble-promotion # warn if float is implicit promoted to double
   -Wformat=2 # warn on security issues around functions that format output (ie printf)
   -Wimplicit-fallthrough # warn on statements that fallthrough without an explicit annotationk
)

add_library(target_options INTERFACE)
target_compile_features(target_options 
   INTERFACE 
      cxx_std_${CMAKE_CXX_STANDARD}
)

target_compile_options(target_options
   INTERFACE
      ${warnings}
)

add_subdirectory(src)
