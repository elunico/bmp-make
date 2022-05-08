project(bmp-make)

set(CMAKE_CXX_STANDARD 20)

add_executable(transform transform.cpp )
add_executable(main main.cpp color.cpp image.cpp)
