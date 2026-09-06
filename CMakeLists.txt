cmake_minimum_required(VERSION 3.14)
project(dtm_tools C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(include)

# Находим графические библиотеки для основной программы
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(dtm_tools 
    src/main.c 
    src/core.c 
    src/csv_parser.c 
    src/triangulation.c 
    src/viewer.c
)
target_link_libraries(dtm_tools PRIVATE ${GLFW_STATIC_LIBRARIES} GL m dl pthread)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(run_tests 
    tests/test_core.cpp 
    src/core.c
)
target_link_libraries(run_tests PRIVATE gtest_main m dl pthread)

include(GoogleTest)
gtest_discover_tests(run_tests)

