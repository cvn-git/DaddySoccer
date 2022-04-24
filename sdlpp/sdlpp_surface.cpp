#include <sdlpp_surface.h>
#include <sdlpp_ttf.h>

#include <stdexcept>

namespace SdlPP {

static SDL_Color createSDLcolor(const Color &color)
{
    SDL_Color sdl;
    sdl.a = color.a_;
    sdl.r = color.r_;
    sdl.g = color.g_;
    sdl.b = color.b_;
    return sdl;
}

//------------------------------------------------------------------------------
Surface::Surface(int width, int height)
{
    surface_ = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (surface_ == nullptr)
        throw std::runtime_error("Cannot create SDL surface");
}

Surface::Surface(const std::string &filename)
{
    surface_ = SDL_LoadBMP(filename.c_str());
    if (surface_ == nullptr)
        throw std::runtime_error("Cannot load BMP file");
}

Surface::Surface(Font &font, const std::string &text, const Color &color)
{
    surface_ = TTF_RenderText_Blended(font.handle(), text.c_str(), createSDLcolor(color));
    if (surface_ == nullptr)
        throw std::runtime_error("Cannot create text surface");
}

Surface::~Surface()
{
    if (surface_ != nullptr)
        SDL_FreeSurface(surface_);
}

void Surface::getSize(int &width, int &height) const
{
    width = surface_->w;
    height = surface_->h;
}

void Surface::blitTo(Surface &dst, SDL_Rect *srcRect, SDL_Rect *dstRect)
{
    if (SDL_BlitSurface(surface_, srcRect, dst.handle(), dstRect) != 0)
        throw std::runtime_error("Cannot bit surface");
}


}   // SdlPP namespace
