#ifndef SDLPP_RENDERER_H
#define SDLPP_RENDERER_H

#include <sdlpp_pixels.h>

#include <SDL_render.h>

#include <string>

namespace SdlPP {

class Window;   // forward declaration
class Surface;  // forward declaration
class Texture;  // forward declaration

class Renderer
{
public:
    Renderer(Window &window, Uint32 flags);
    Renderer(Surface &surface);
    Renderer(const Renderer &other) = delete;
    ~Renderer();

    Renderer& operator=(const Renderer &other) = delete;

    SDL_Renderer* handle() const {return renderer_;}

    void setTarget(Texture *texture);

    void setDrawBlendMode(SDL_BlendMode blendMode);
    void setDrawColor(const Color &color);

    void clear(const Color &color);
    void copy(Texture &texture, const SDL_Rect *src, const SDL_Rect *dst);
    void tile(Texture &tile, const SDL_Rect &dst);
    void drawLine(int x1, int y1, int x2, int y2, const Color &color);
    void drawRect(const SDL_Rect &rect, const Color &color);
    void fillRect(const SDL_Rect &rect, const Color &color);
    void present();

private:
    SDL_Renderer *renderer_ = nullptr;
};

class Texture
{
public:
    Texture(Renderer &renderer,int width, int height, Uint32 format = SDL_PIXELFORMAT_RGBA32, int access = SDL_TEXTUREACCESS_TARGET);
    Texture(Renderer &renderer, const Surface &surface);
    Texture(Renderer &renderer, const std::string &filename);
    Texture(const Texture &other) = delete;
    ~Texture();

    Texture& operator=(const Texture &other) = delete;

    void writePixels(const SDL_Rect* rect, const void* pixels, int pitch);

    Renderer& renderer() const {return renderer_;}
    SDL_Texture* handle() const {return texture_;}

    void getSize(int &width, int &height) const;

private:
    Renderer &renderer_;
    SDL_Texture *texture_ = nullptr;
};


}   // SdlPP namespace

#endif  // SDLPP_RENDERER_H
