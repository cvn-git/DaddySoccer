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
)

target_link_libraries(SdlPP
	PUBLIC SDL2_ttf
)
