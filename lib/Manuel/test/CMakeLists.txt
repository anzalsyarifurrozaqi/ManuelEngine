# Setup
find_package(Catch2 3 REQUIRED)

# Declaration
# add_test(NAME MANUEL_TEST COMMAND Test_Manuel)
add_executable(Test_Manuel)

# Main Definition
target_sources(
    Test_Manuel 
    PRIVATE Logger.cpp
            ManagedMemory.cpp
)

target_link_libraries(Test_Manuel PRIVATE Catch2::Catch2WithMain Manuel)
