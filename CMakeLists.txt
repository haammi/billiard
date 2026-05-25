cmake_minimum_required(VERSION 3.16)
project(billiard C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.3
)
FetchContent_MakeAvailable(SDL2)

add_executable(billiard
    src/main.c
    src/game.c
)

target_include_directories(billiard PRIVATE src)
target_link_libraries(billiard PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Копируем SDL2.dll на Windows
if(WIN32)
    add_custom_command(TARGET billiard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:billiard>
    )
endif()