cmake_minimum_required(VERSION 3.30)
project(CppNatsProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
#include(CTest)

#include(CMakePrintHelpers)
include(FetchContent)
#include(GenerateExportHeader)

#set(default_build_type "Debug")

# Fetch cnats as a dependency
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
  cnats
  GIT_REPOSITORY https://github.com/nats-io/nats.c.git
  GIT_TAG        v3.12.0
  GIT_SHALLOW    TRUE
)

set(NATS_BUILD_WITH_TLS OFF)
set(NATS_BUILD_EXAMPLES OFF)
set(NATS_BUILD_STREAMING OFF)
set(NATS_BUILD_LIB_SHARED OFF)
set(BUILD_TESTING ON) # Disable building tests for cnats

FetchContent_MakeAvailable(cnats)

include_directories(${cnats_SOURCE_DIR}/src src)

# cppnats library 
#file(GLOB SOURCES "src/*.cpp")
#file(GLOB HEADERS "src/*.h")
add_library(cppnats STATIC
    src/helper.cpp
    src/cppnats.cpp)
target_include_directories(cppnats PUBLIC include)
target_link_libraries(cppnats nats_static)

# doctest
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.12
)
FetchContent_MakeAvailable(doctest)

# Tests
add_executable(cppnats_tests
    tests/test_main.cpp
    tests/test_core.cpp
    tests/test_jetstream.cpp
)

target_link_libraries(cppnats_tests
    PRIVATE
        cppnats
        doctest::doctest
)
target_compile_definitions(cppnats_tests PRIVATE
    NATS_SERVER_PATH="${CMAKE_BINARY_DIR}/bin/nats-server"
)
# Register tests with CTest
add_test(NAME cppnats_tests COMMAND cppnats_tests)
