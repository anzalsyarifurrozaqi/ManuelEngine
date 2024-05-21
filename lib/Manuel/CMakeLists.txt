cmake_minimum_required(VERSION 3.15)
project(Manuel CXX)

# Dependencies
find_package(fmt REQUIRED)

# Declaration
add_library(Manuel SHARED)

message("pulic folder ${CMAKE_CURRENT_BINARY_DIR}")

# Definition
add_subdirectory(src)

generate_export_header(Manuel EXPORT_MACRO_NAME MANUEL_EXPORT EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/Public/Manuel/Export.h)

target_include_directories(Manuel PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/Public)

target_link_libraries(Manuel PUBLIC fmt::fmt)

if(UNIX)
    target_compile_options(Manuel PRIVATE -fPIC)
    target_link_libraries(Manuel PUBLIC dl pthread)
endif()

if(MSVC)
    target_compile_options(Manuel PRIVATE /EHsc)
else()
    target_compile_options(Manuel PRIVATE -fno-exceptions)
endif()

# Auxiliary Targets
if(BUILD_TESTS)
    add_subdirectory(test)
endif()