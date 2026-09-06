cmake_minimum_required(VERSION 3.20)
project(disk_report)
set(CMAKE_CXX_STANDARD 23)

set(
        SOURCE_FILES main.cpp
)

add_executable(
        disk_report
        ${SOURCE_FILES}
)

if (MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    target_link_libraries(${PROJECT_NAME} stdc++exp) # std:: print, println, ...
endif()