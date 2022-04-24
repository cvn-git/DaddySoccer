cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MINGW)
    set(SDL2_PATH "C:/SDL2-mingw/i686-w64-mingw32")
    set(SDL2_TTF_PATH "C:/SDL2_ttf-mingw/i686-w64-mingw32")
elseif (WIN32)
    set(SDL2_PATH "C:/SDL2")
    set(SDL2_TTF_PATH "C:/SDL2_ttf")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../cmake-packages")
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Copy SDL2.dll (Windows only)
if (MINGW)
    get_filename_component(SDL2_LIB_PATH ${SDL2MAIN_LIBRARY} DIRECTORY)
    set(SDL2_LIB_PATH "${SDL2_LIB_PATH}/../bin")
    message("Copy SDL2.dll from ${SDL2_LIB_PATH}")
    configure_file("${SDL2_LIB_PATH}/SDL2.dll" "${CMAKE_BINARY_DIR}" COPYONLY)

    get_filename_component(SDL2_TTF_LIB_PATH ${SDL2_TTF_LIBRARY} DIRECTORY)
    set(SDL2_TTF_LIB_PATH "${SDL2_TTF_LIB_PATH}/../bin")
    message("Copy DLLs from ${SDL2_TTF_LIB_PATH}")
    configure_file("${SDL2_TTF_LIB_PATH}/SDL2_ttf.dll" "${CMAKE_BINARY_DIR}" COPYONLY)
    configure_file("${SDL2_TTF_LIB_PATH}/libfreetype-6.dll" "${CMAKE_BINARY_DIR}" COPYONLY)
    configure_file("${SDL2_TTF_LIB_PATH}/zlib1.dll" "${CMAKE_BINARY_DIR}" COPYONLY)
elseif (WIN32)
    get_filename_component(SDL2_LIB_PATH ${SDL2MAIN_LIBRARY} DIRECTORY)
    message("Copy SDL2.dll from ${SDL2_LIB_PATH}")
    configure_file("${SDL2_LIB_PATH}/SDL2.dll" "${CMAKE_BINARY_DIR}" COPYONLY)

    get_filename_component(SDL2_TTF_LIB_PATH ${SDL2_TTF_LIBRARY} DIRECTORY)
    message("Copy DLLs from ${SDL2_TTF_LIB_PATH}")
    configure_file("${SDL2_TTF_LIB_PATH}/SDL2_ttf.dll" "${CMAKE_BINARY_DIR}" COPYONLY)
    configure_file("${SDL2_TTF_LIB_PATH}/libfreetype-6.dll" "${CMAKE_BINARY_DIR}" COPYONLY)
    configure_file("${SDL2_TTF_LIB_PATH}/zlib1.dll" "${CMAKE_BINARY_DIR}" COPYONLY)
endif()

set(SDLPP_SOURCES
    sdlpp.h
    sdlpp_common.h
    sdlpp_joystick.h    sdlpp_joystick.cpp
    sdlpp_pixels.h      sdlpp_pixels.cpp
    sdlpp_renderer.h    sdlpp_renderer.cpp
    sdlpp_surface.h     sdlpp_surface.cpp
    sdlpp_timer.h
    sdlpp_ttf.h         sdlpp_ttf.cpp
    sdlpp_video.h       sdlpp_video.cpp
)

# library
add_library(SdlPP
    ${SDLPP_SOURCES}
)

target_include_directories(SdlPP
    PUBLIC .
    PUBLIC ${SDL2_INCLUDE_DIR}
    PUBLIC ${SDL2_TTF_INCLUDE_DIR}
)

target_link_libraries(SdlPP
    PUBLIC ${SDL2_LIBRARY}
    PUBLIC ${SDL2_TTF_LIBRARY}
)
