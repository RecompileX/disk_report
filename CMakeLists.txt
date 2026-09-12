cmake_minimum_required(VERSION 3.20)
project(disk_report)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
        SOURCE_FILES
        src/main.cpp
)

add_compile_definitions(
        UNICODE
        _UNICODE
)

add_executable(
        disk_report
        ${SOURCE_FILES}
)

target_include_directories(
        disk_report PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

if (MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    target_link_libraries(
            disk_report
            stdc++exp
    )

    # Find CLion's MinGW bin folder.
    get_filename_component(
            MINGW_BIN_DIR
            "${CMAKE_CXX_COMPILER}"
            DIRECTORY
    )

    # Copy MinGW runtime DLLs beside disk_report.exe.
    add_custom_command(
            TARGET disk_report
            POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "$<TARGET_FILE_DIR:disk_report>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "$<TARGET_FILE_DIR:disk_report>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
            "$<TARGET_FILE_DIR:disk_report>"
    )

endif()