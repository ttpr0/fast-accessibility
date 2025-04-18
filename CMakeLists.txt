cmake_minimum_required(VERSION 3.21)
project(test LANGUAGES CXX)

set(pyaccess_VERSION 0.0.1a)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_INSTALL_RPATH $ORIGIN)

#***********************************
# find dependencies
#***********************************

# set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG RELEASE)

find_package(Eigen3 REQUIRED)
find_package(OpenMP REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

#***********************************
# library
#***********************************
set(lib_name access)

add_library(${lib_name} SHARED
    src/graph/base_graph.cpp
    src/graph/ch_graph.cpp
    src/graph/ch_graph_2.cpp
    src/graph/tiled_graph.cpp
    src/graph/transit_graph.cpp
    src/graph/structs/adjacency.cpp
    src/graph/preproc/build_ch_index.cpp
    src/graph/preproc/node_ordering.cpp
    src/graph/explorer/base_explorer.cpp
    src/graph/explorer/tiled_explorer.cpp
    src/graph/explorer/ch_explorer.cpp
    src/graph/base/graph_index.cpp
    src/graph/base/graph_base.cpp
    src/graph/weights/weighting.cpp
    src/graph/weights/tc_weighting.cpp
    src/graph/weights/transit_weighting.cpp
    src/graph/io/cell_index_io.cpp
    src/graph/io/ch_data_io.cpp
    src/graph/io/graph_base_io.cpp
    src/graph/io/id_mapping_io.cpp
    src/graph/io/partition_io.cpp
    src/graph/io/tiled_data_io.cpp
    src/graph/io/weighting_io.cpp
    src/graph/io/transit_data_io.cpp
    src/graph/io/loader.cpp
    src/graph/mod/adjacency_mod.cpp
    src/graph/mod/ch_mod.cpp
    src/graph/mod/graph_base_mod.cpp
    src/graph/mod/id_mapping_mod.cpp
    src/graph/mod/overlay_mod.cpp
    src/graph/mod/partition_mod.cpp
    src/graph/mod/weighting_mod.cpp
    src/util/kd_tree/kd_tree.cpp
    src/util/osm/graph_parser.cpp
    src/util/osm/parser.cpp
    src/util/osm/proto/fileformat.pb.cc
    src/util/osm/proto/osmformat.pb.cc
)

target_include_directories(${lib_name} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:pyaccess/include>
)
target_link_libraries(${lib_name} PRIVATE
    OpenMP::OpenMP_CXX
    Eigen3::Eigen
    protobuf::libprotobuf
    ZLIB::ZLIB
)

#***********************************
# benchmark executable
#***********************************
set(exe_name test)

add_executable(${exe_name}
    src/main.cpp
)

add_dependencies(${exe_name} ${lib_name})
target_include_directories(${exe_name} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${exe_name} PRIVATE
    ${lib_name}
    OpenMP::OpenMP_CXX
    Eigen3::Eigen
)

#***********************************
# binding for library
#***********************************
set(ext_name _pyaccess_ext)

# set(CMAKE_BUILD_TYPE Release)
find_package(Python 3.10 EXACT COMPONENTS Interpreter Development.Module REQUIRED)
# set(CMAKE_BUILD_TYPE Debug)

find_package(nanobind CONFIG REQUIRED)

nanobind_add_module(${ext_name}
    src/bind.cpp
)

add_dependencies(${ext_name} ${lib_name})
target_link_libraries(${ext_name} PRIVATE
    ${lib_name}
    Eigen3::Eigen
)

#***********************************
# install targets
#***********************************
# set install path to current directory if not overwritten
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "install prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# install libraries
install(
    TARGETS ${lib_name}
    RUNTIME_DEPENDENCY_SET access_dependencies
    EXPORT PyAccessTargets
    RUNTIME DESTINATION pyaccess
    LIBRARY DESTINATION pyaccess
    ARCHIVE DESTINATION pyaccess/lib
)
install(DIRECTORY include/pyaccess DESTINATION pyaccess/include)
install(TARGETS ${ext_name} LIBRARY DESTINATION pyaccess)

# install exported targets
install(
    EXPORT PyAccessTargets
    FILE PyAccessTargets.cmake
    DESTINATION pyaccess/cmake
    NAMESPACE pyaccess::
)

# install cmake config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/pyaccess-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/pyaccess-config.cmake
    INSTALL_DESTINATION pyaccess/cmake
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/pyaccess-config-version.cmake
    VERSION ${pyaccess_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/pyaccess-config.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/pyaccess-config-version.cmake
    DESTINATION pyaccess/cmake
)
