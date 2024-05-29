cmake_minimum_required(VERSION 3.25)
project(Manuel-Engine CXX)

# Global Option
include(CMakeDependentOption)
option(BUILD_TEST "Build test programs" OFF)
option(ONLY_COVERAGE "Build only tests necessary for coverage" FALSE)
option(LIBCPP "Build with libc++" FALSE)
option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
option(ENABLE_ASAN "Enable address sanitizer" FALSE)
option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" FALSE)
option(ENABLE_TESTING "Enable the building of the test" FALSE)
option(ENABLE_CLANG_TIDY "Enable testing with clang-tidy" FALSE)
option(ENABLE_CPPCHECK "Enable testing with cppcheck" FALSE)
option(SIMPLE_BUILD "Build the project as minimally as possible" FALSE)
option(BUILD_DOC "Build the project's documentation" ON)
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
option(DEBUG_LOGGING "Enabling debug logging" FALSE)

# C11 standard, no extensions
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# C++20 standard, no extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header Exports
include(GenerateExportHeader)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

if(BUILD_TESTS)
  enable_testing()
endif()

# Libraries
add_subdirectory(lib)

# Test
find_package(fmt REQUIRED)

# Engine Entrypoint
add_executable(Manuel_Bringup)

# Source
add_subdirectory(src)

# Test
target_link_libraries(Manuel_Bringup PRIVATE fmt::fmt)


# Summery
message("#########################################")
message("\t \t Summary")
message("#########################################")
message("Build type:       \t ${CMAKE_BUILD_TYPE}")
message("Install prefix:   \t ${CMAKE_INSTALL_PREFIX}")
message("Testing enabled:  \t ${ENABLE_TESTING}")
message("Clang-tidy:       \t ${ENABLE_CLANG_TIDY}")
message("Cppcheck:         \t ${ENABLE_CPPCHECK}")
message("Compiler:         \t ${CMAKE_CXX_COMPILER_ID}")
message("Sanizizers:       \t ${ENABLE_ASAN}")
message("Shared libs:      \t ${BUILD_SHARED_LIBS}")
message("Build libcpp:     \t ${LIBCPP}")
message("CCache executable:\t ${CCACHE}")
message("Building doc:     \t ${BUILD_DOC}")
message("BUILD_TEST:       \t ${BUILD_TEST}")
message("------------------------------------------")
message("Version:          \t ${PROJECT_VERSION}")
message("########################################")