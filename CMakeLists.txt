cmake_minimum_required(VERSION 3.10)
project(dtm_tools C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(include)

file(GLOB SOURCES "src/*.c")
add_executable(dtm_tools ${SOURCES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

target_link_libraries(dtm_tools 
    PRIVATE 
    ${GLFW_STATIC_LIBRARIES} 
    GL 
    m 
    dl 
    pthread
)

