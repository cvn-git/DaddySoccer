#ifndef SDLPP_COMMON_H
#define SDLPP_COMMON_H

#include <SDL_stdinc.h>
#include <SDL_rect.h>

namespace SdlPP {

inline
SDL_Rect createRect(int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    return rect;
}


}   // SdlPP namespace

#endif  // SDLPP_COMMON_H
