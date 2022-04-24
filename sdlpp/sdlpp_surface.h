#ifndef SDLPP_SURFACE_H
#define SDLPP_SURFACE_H

#include <sdlpp_common.h>
#include <sdlpp_pixels.h>

#include <SDL_surface.h>

#include <string>

namespace SdlPP {

class Font; // forward declaration

class Surface
{
public:
    Surface(int width, int height);
    Surface(const std::string &filename);
    Surface(Font &font, const std::string &text, const Color &color);
    Surface(const Surface &other) = delete;
    ~Surface();

    Surface& operator=(const Surface &other) = delete;

    SDL_Surface* handle() const {return surface_;}

    void getSize(int &width, int &height) const;
    void blitTo(Surface &dst, SDL_Rect *srcRect, SDL_Rect *dstRect);

private:
    SDL_Surface *surface_ = nullptr;
};


}   // SdlPP namespace

#endif  // SDLPP_SURFACE_H
