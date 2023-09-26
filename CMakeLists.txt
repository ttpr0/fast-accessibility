cmake_minimum_required(VERSION 3.8)
project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

#***********************************
# benchmark executable
#***********************************
set(exe_name test)

add_executable(${exe_name}
    src/main.cpp
    src/graph/base_graph.cpp
    src/graph/ch_graph.cpp
    src/graph/ch_graph_2.cpp
    src/graph/tiled_graph.cpp
    src/graph/loader.cpp
    src/graph/topology_storage.cpp
    src/kd_tree/kd_tree.cpp
)

target_include_directories(${exe_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${exe_name} PUBLIC OpenMP::OpenMP_CXX)
endif()

#***********************************
# library
#***********************************
set(lib_name libaccess)

add_library(${lib_name} SHARED
    src/graph/base_graph.cpp
    src/graph/ch_graph.cpp
    src/graph/ch_graph_2.cpp
    src/graph/tiled_graph.cpp
    src/graph/loader.cpp
    src/graph/topology_storage.cpp
    src/kd_tree/kd_tree.cpp
)

target_include_directories(${lib_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${lib_name} PUBLIC OpenMP::OpenMP_CXX)
endif()

#***********************************
# binding for library
#***********************************
set(ext_name _pyaccess_ext)

find_package(Python 3.10 COMPONENTS Interpreter Development.Module REQUIRED)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")
find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(${ext_name}
    src/bind.cpp
)

target_include_directories(${ext_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_dependencies(${ext_name} ${lib_name})
target_link_libraries(${ext_name} PRIVATE
    ${lib_name}
)

#***********************************
# install targets
#***********************************
# set install path to current directory if not overwritten
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# install libraries
install(TARGETS ${lib_name} DESTINATION pyaccess)
install(TARGETS ${ext_name} LIBRARY DESTINATION pyaccess)