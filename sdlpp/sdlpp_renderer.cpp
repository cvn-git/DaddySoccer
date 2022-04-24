#include <sdlpp_renderer.h>
#include <sdlpp_video.h>
#include <sdlpp_surface.h>

#include <iostream>
#include <algorithm>

namespace SdlPP {

//------------------------------------------------------------------------------
Renderer::Renderer(Window &window, Uint32 flags)
{
    renderer_ = SDL_CreateRenderer(window.handle(), -1, flags);
    if (renderer_ == nullptr)
        throw std::invalid_argument("Null renderer");

    setDrawBlendMode(SDL_BLENDMODE_NONE);
}

Renderer::Renderer(Surface &surface)
{
    renderer_ = SDL_CreateSoftwareRenderer(surface.handle());
    if (renderer_ == nullptr)
        throw std::runtime_error("Cannot create SDL software renderer");
}

Renderer::~Renderer()
{
    if (renderer_ != nullptr)
        SDL_DestroyRenderer(renderer_);
}

void Renderer::setTarget(Texture *texture)
{
    SDL_Texture* ptr = (texture != nullptr)? texture->handle() : NULL;
    if (SDL_SetRenderTarget(renderer_, ptr) != 0)
        throw std::runtime_error("SDL_SetRenderTarget() failed");
}

void Renderer::setDrawBlendMode(SDL_BlendMode blendMode)
{
    if (renderer_ != nullptr)
        SDL_SetRenderDrawBlendMode(renderer_, blendMode);
}

void Renderer::setDrawColor(const Color &color)
{
    if (renderer_ != nullptr)
        SDL_SetRenderDrawColor(renderer_, color.r_, color.g_, color.b_, color.a_);
}

void Renderer::clear(const Color &color)
{
    if (renderer_ != nullptr)
    {
        setDrawColor(color);
        SDL_RenderClear(renderer_);
    }
}

void Renderer::copy(Texture &texture, const SDL_Rect *src, const SDL_Rect *dst)
{
    if (SDL_RenderCopy(renderer_, texture.handle(), src, dst) != 0)
    {
        std::cout << "SDL ERROR: " << SDL_GetError() << std::endl;
        throw std::runtime_error("Cannot copy texture to renderer");
    }
}

void Renderer::tile(Texture &tile, const SDL_Rect &dst)
{
    int tileWidth = 0, tileHeight = 0;
    tile.getSize(tileWidth, tileHeight);

    SDL_Rect srcRect, dstRect;
    srcRect.x = 0;
    srcRect.y = 0;

    int y = 0;
    while (y < dst.h)
    {
        dstRect.y = y + dst.y;
        srcRect.h = std::min(dst.h - y, tileHeight);
        dstRect.h = srcRect.h;

        int x = 0;
        while (x < dst.w)
        {
            dstRect.x = x + dst.x;
            srcRect.w = std::min(dst.w - x, tileWidth);
            dstRect.w = srcRect.w;

            copy(tile, &srcRect, &dstRect);

            x += tileWidth;
        }

        y += tileHeight;
    }
}

void Renderer::drawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    if (renderer_ != nullptr)
    {
        setDrawColor(color);
        SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
    }
}

void Renderer::drawRect(const SDL_Rect &rect, const Color &color)
{
    if (renderer_ != nullptr)
    {
        setDrawColor(color);
        SDL_RenderDrawRect(renderer_, &rect);
    }
}

void Renderer::fillRect(const SDL_Rect &rect, const Color &color)
{
    if (renderer_ != nullptr)
    {
        setDrawColor(color);
        SDL_RenderFillRect(renderer_, &rect);
    }
}

void Renderer::present()
{
    if (renderer_ != nullptr)
        SDL_RenderPresent(renderer_);
}

//------------------------------------------------------------------------------
Texture::Texture(Renderer &renderer,int width, int height, Uint32 format, int access)
    : renderer_(renderer)
{
    texture_ = SDL_CreateTexture(renderer.handle(), format, access, width, height);
    if (texture_ == nullptr)
        throw std::invalid_argument("SDL_CreateTexture() failed");
}

Texture::Texture(Renderer &renderer, const Surface &surface)
    : renderer_(renderer)
{
    texture_ = SDL_CreateTextureFromSurface(renderer.handle(), surface.handle());
    if (texture_ == nullptr)
        throw std::invalid_argument("SDL_CreateTextureFromSurface() failed");
}

Texture::Texture(Renderer &renderer, const std::string &filename)
    : Texture(renderer, Surface(filename))
{
}

Texture::~Texture()
{
    if (texture_ != nullptr)
        SDL_DestroyTexture(texture_);
}

void Texture::writePixels(const SDL_Rect* rect, const void* pixels, int pitch)
{
    if (texture_ != nullptr)
    {
        if (SDL_UpdateTexture(texture_, rect, pixels, pitch) != 0)
            throw std::runtime_error("SDL_UpdateTexture() failed");
    }
}

void Texture::getSize(int &width, int &height) const
{
    int access;
    Uint32 format;
    if (SDL_QueryTexture(texture_, &format, &access, &width, &height))
        throw std::runtime_error("Cannot query SDL texture");
}


}   // SdlPP namespace
