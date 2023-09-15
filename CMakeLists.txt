cmake_minimum_required(VERSION 3.8)
project(test LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/graph/base_graph.cpp
    src/graph/ch_graph.cpp
    src/graph/loader.cpp
    src/graph/topology_storage.cpp
    src/kd_tree/kd_tree.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
