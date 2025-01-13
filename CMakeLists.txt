cmake_minimum_required(VERSION 3.10)

project(PingPongDetection)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_subdirectory(CircleDetection)
add_subdirectory(ColorMaskProcessor)

add_executable(main main.cpp)
target_link_libraries(main
    CircleDetection
    ColorMaskProcessor
    ${OpenCV_LIBS}
)
