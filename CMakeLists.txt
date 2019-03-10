cmake_minimum_required(VERSION 3.13)
project(KAWORU)

set(CMAKE_CXX_STANDARD 17)

add_executable(KAWORU main.cpp tools_CNN.h tools_general.h tools_file.h)
find_package(OpenCV REQUIRED)
target_link_libraries(KAWORU ${OpenCV_LIBS})