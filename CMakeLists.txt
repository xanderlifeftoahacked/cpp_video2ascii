cmake_minimum_required(VERSION 3.10)
project(ASCII_video)

find_package(OpenCV REQUIRED)
find_package(Curses REQUIRED)
find_package(VTK REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${CURSES_INCLUDE_DIRS} ${VTK_INCLUDE_DIRS})

add_executable(MyExecutable src/main.cpp)

target_link_libraries(MyExecutable ${OpenCV_LIBS} ${VTK_LIBRARIES} ${CURSES_LIBRARIES})

target_compile_definitions(MyExecutable PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")

