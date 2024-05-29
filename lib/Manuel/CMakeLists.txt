cmake_minimum_required(VERSION 3.25)
project(Manuel CXX)

# Dependencies
find_package(fmt REQUIRED)

# Declaration
add_library(Manuel SHARED)

# Definition
add_subdirectory(src)

generate_export_header(Manuel EXPORT_MACRO_NAME MANUEL_EXPORT EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/Public/Manuel/Export.hpp)

target_include_directories(Manuel PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/Public)

target_link_libraries(Manuel PUBLIC fmt::fmt)

target_compile_options(Manuel PRIVATE -fno-exceptions)

if(BUILD_TEST)
    add_subdirectory(test)
endif()