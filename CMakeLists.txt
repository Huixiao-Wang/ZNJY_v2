cmake_minimum_required(VERSION 3.10)
project(PingPongDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED)

# 添加子目录
add_subdirectory(CircleDetection)

# 主程序
add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS} CircleDetection)
