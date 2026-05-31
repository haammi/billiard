cmake_minimum_required(VERSION 3.16)
project(billiard C)

set(CMAKE_C_STANDARD 11)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL_ARCH "x86_64-w64-mingw32")
    else()
        set(SDL_ARCH "i686-w64-mingw32")
    endif()

    set(SDL2_DIR       "${CMAKE_SOURCE_DIR}/libs/SDL2/${SDL_ARCH}")
    set(SDL2_TTF_DIR   "${CMAKE_SOURCE_DIR}/libs/SDL2_ttf/${SDL_ARCH}")
    set(SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2_image/${SDL_ARCH}")
    set(SDL2_MIXER_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2_mixer/${SDL_ARCH}")

    include_directories(
        ${SDL2_DIR}/include/SDL2
        ${SDL2_TTF_DIR}/include/SDL2
        ${SDL2_IMAGE_DIR}/include/SDL2
        ${SDL2_MIXER_DIR}/include/SDL2
    )

    find_library(SDL2_LIB       SDL2       PATHS ${SDL2_DIR}/lib       NO_DEFAULT_PATH)
    find_library(SDL2MAIN_LIB   SDL2main   PATHS ${SDL2_DIR}/lib       NO_DEFAULT_PATH)
    find_library(SDL2_TTF_LIB   SDL2_ttf   PATHS ${SDL2_TTF_DIR}/lib   NO_DEFAULT_PATH)
    find_library(SDL2_IMAGE_LIB SDL2_image PATHS ${SDL2_IMAGE_DIR}/lib NO_DEFAULT_PATH)
    find_library(SDL2_MIXER_LIB SDL2_mixer PATHS ${SDL2_MIXER_DIR}/lib NO_DEFAULT_PATH)

    set(SDL_LIBS ${SDL2_LIB} ${SDL2_TTF_LIB} ${SDL2_IMAGE_LIB} ${SDL2_MIXER_LIB})

    file(GLOB ALL_DLLS
        "${SDL2_DIR}/bin/*.dll"
        "${SDL2_TTF_DIR}/bin/*.dll"
        "${SDL2_IMAGE_DIR}/bin/*.dll"
        "${SDL2_MIXER_DIR}/bin/*.dll"
    )

else()
    include(FetchContent)

    FetchContent_Declare(SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.3)
    FetchContent_MakeAvailable(SDL2)

    FetchContent_Declare(SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        GIT_TAG release-2.22.0)
    FetchContent_MakeAvailable(SDL2_ttf)

    FetchContent_Declare(SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.2)
    FetchContent_MakeAvailable(SDL2_image)

    FetchContent_Declare(SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.0)

    set(SDL2MIXER_OPUS    OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_FLAC    OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_MOD     OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_MIDI    OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_MP3     OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_OGG     OFF CACHE BOOL "" FORCE)
    set(SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SDL2_mixer)

    set(SDL_LIBS SDL2::SDL2 SDL2::SDL2main SDL2_ttf SDL2_image SDL2_mixer)
endif()

add_executable(billiard
    src/main.c
    src/game.c
    src/ball.c
    src/physics.c
    src/input.c
    src/table.c
    src/hud.c
    src/audio.c
    src/menu.c
)

target_include_directories(billiard PRIVATE src)
target_link_libraries(billiard PRIVATE ${SDL_LIBS} m)

if(WIN32)
    target_link_options(billiard PRIVATE -mconsole)
endif()

if(WIN32)
    add_custom_command(TARGET billiard POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${ALL_DLLS}
            $<TARGET_FILE_DIR:billiard>
    )
endif()

add_custom_command(TARGET billiard POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:billiard>/assets
)
