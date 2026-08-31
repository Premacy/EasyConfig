cmake_minimum_required(VERSION 3.14)
project(EasyConfigTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(easy_config_tests tests.cpp)
target_include_directories(easy_config_tests PRIVATE ../src)
target_compile_definitions(
    easy_config_tests
    PRIVATE TEST_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/config.txt"
)
target_link_libraries(easy_config_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(easy_config_tests)
