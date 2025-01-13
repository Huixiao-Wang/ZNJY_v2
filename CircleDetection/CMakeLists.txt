cmake_minimum_required(VERSION 3.10)
project(CircleDetection)

set(CMAKE_CXX_STANDARD 17)

# 包含路径
include_directories(${OpenCV_INCLUDE_DIRS} include)

# 创建静态库
add_library(CircleDetection STATIC src/CircleDetector.cpp)
target_link_libraries(CircleDetection ${OpenCV_LIBS})
