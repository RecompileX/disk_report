cmake_minimum_required(VERSION 3.20)

project(disk_report)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    src/main.cpp
)

add_executable(
    disk_report
    ${SOURCE_FILES}
)

target_include_directories(
    disk_report PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_definitions(
    disk_report PRIVATE
    UNICODE
    _UNICODE
    NOMINMAX
)