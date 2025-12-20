cmake_minimum_required(VERSION 3.16...3.27)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SpcGen)

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

add_subdirectory(global)
add_subdirectory(core)
add_subdirectory(parser)
add_subdirectory(device)

add_executable(SpcGen SpcGen.cpp)

target_link_libraries(SpcGen PRIVATE
    global
    core
    parser
    device
    ${ROOT_LIBRARIES}
)