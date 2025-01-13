cmake_minimum_required(VERSION 3.10)

project(ColorMaskProcessor)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

# 包含路径
include_directories(${OpenCV_INCLUDE_DIRS} include)

# 创建静态库
add_library(ColorMaskProcessor STATIC src/ColorMaskProcessor.cpp)
target_link_libraries(ColorMaskProcessor ${OpenCV_LIBS})
