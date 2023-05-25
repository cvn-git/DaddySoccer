set(SDL_SHARED OFF CACHE BOOL "Build a shared version of the library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build a static version of the library" FORCE)
add_subdirectory(SDL)

set(BUILD_SHARED_LIBS OFF)
add_subdirectory(SDL_ttf)
add_subdirectory(SDL_ttf/external/freetype-2.11.1)
