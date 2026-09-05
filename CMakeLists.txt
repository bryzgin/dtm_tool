cmake_minimum_required(VERSION 3.10)
project(dtm_tools C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(include)

# Собираем все .c файлы
file(GLOB SOURCES "src/*.c")
add_executable(dtm_viewer ${SOURCES})

if(WIN32)
    target_link_libraries(dtm_viewer 
        PRIVATE 
        glfw3 
        opengl32 
        gdi32 
        user32 
        kernel32 
        m
    )
    
    target_link_options(dtm_viewer PRIVATE "-static")
    
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    
    target_link_libraries(dtm_viewer 
        PRIVATE 
        ${GLFW_STATIC_LIBRARIES} 
        GL 
        m 
        dl 
        pthread
    )
endif()

