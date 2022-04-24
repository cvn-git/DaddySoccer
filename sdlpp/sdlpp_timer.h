#ifndef SDLPP_TIMER_H
#define SDLPP_TIMER_H

#include <SDL.h>


namespace SdlPP {

inline double getCurrentTime()
{
    return SDL_GetTicks() * 1e-3;
}


}   // SdlPP namespace

#endif  // SDLPP_TIMER_H
