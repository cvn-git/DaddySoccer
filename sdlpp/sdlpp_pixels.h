#ifndef SDLPP_PIXELS_H
#define SDLPP_PIXELS_H

#include <SDL_pixels.h>

namespace SdlPP {

struct Color
{
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r_(r), g_(g), b_(b), a_(a) {}
    Color(Uint8 r, Uint8 g, Uint8 b) : r_(r), g_(g), b_(b), a_(255) {}
    Uint8 r_ = 0;
    Uint8 g_ = 0;
    Uint8 b_ = 0;
    Uint8 a_ = 0;
};

class PixelFormat
{
public:
    PixelFormat(Uint32 pixel_format=SDL_PIXELFORMAT_RGBA32);
    PixelFormat(const PixelFormat &other) = delete;
    ~PixelFormat();
    
    PixelFormat& operator=(const PixelFormat &other) = delete;
    
    Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b) const;
    
private:
    SDL_PixelFormat *format_;    
};


}   // SdlPP namespace

#endif  // SDLPP_PIXELS_H
