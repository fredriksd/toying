
add_executable(x main.cpp)

target_link_libraries(x
   PUBLIC
      target_options
   PRIVATE
   sfml-graphics
   sfml-window
   sfml-system
)

