cmake_minimum_required(VERSION 3.25)
project(ASCII_video)
set(OpenCV_DIR /usr/lib/opencv4/opencv2)
find_package(OpenCV REQUIRED)
# find_package(VTK REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(MyExecutable src/main.cpp) 
target_link_libraries(MyExecutable ${OpenCV_LIBS} ${VTK_LIBRARIES})
target_compile_definitions(MyExecutable PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

