#include <sdlpp_pixels.h>

#include <stdexcept>

namespace SdlPP {

PixelFormat::PixelFormat(Uint32 pixel_format)
{
    format_ = SDL_AllocFormat(pixel_format);
    if (format_ == nullptr)
    {
        throw std::runtime_error("SDL_AllocFormat() failed");
    }
}

PixelFormat::~PixelFormat()
{
    if (format_ != nullptr)
    {
        SDL_FreeFormat(format_);
    }
}

Uint32 PixelFormat::mapRGB(Uint8 r, Uint8 g, Uint8 b) const
{
    if (format_ == nullptr)
        throw std::runtime_error("PixelFormat not existing");

    return SDL_MapRGB(format_, r, g, b);
}


}   // SdlPP namespace
