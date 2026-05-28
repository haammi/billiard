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

FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0
)
FetchContent_MakeAvailable(SDL2_ttf)

FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.8.0
)
# Disable sound formats except wav
set(SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_FLAC OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MOD  OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MIDI OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MP3  OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_OGG  OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(SDL2_mixer)

add_executable(billiard
    src/main.c
    src/game.c
    src/ball.c
    src/physics.c
    src/input.c
    src/table.c
    src/hud.c
    src/audio.c
)

target_include_directories(billiard PRIVATE src)
target_link_libraries(billiard PRIVATE SDL2::SDL2 SDL2::SDL2main SDL2_ttf SDL2_mixer m)
# Копируем SDL2.dll на Windows
if(WIN32)
    add_custom_command(TARGET billiard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:billiard>
    )
endif()
