#ifndef SDLPP_VIDEO_H
#define SDLPP_VIDEO_H

#include <sdlpp_renderer.h>

#include <SDL_video.h>

#include <string>
#include <memory>

namespace SdlPP {

class Window
{
public:
    Window(const std::string &title, int width, int height, Uint32 windowFlags, Uint32 rendererFlags);
    Window(const Window &other) = delete;
    ~Window();

    Window& operator=(const Window &other) = delete;

    void getSize(int &width, int &height) const;
    SDL_Rect getSize() const;

    Renderer& renderer() {return *renderer_;}
    void setRendererTarget(Texture &texture) {renderer_->setTarget(&texture);}
    void restoreDefaultRendererTarget() {renderer_->setTarget(nullptr);}

    SDL_Window* handle() const {return window_;}

protected:
    std::unique_ptr<Renderer> renderer_;

private:
    SDL_Window *window_ = nullptr;
};


}   // SdlPP namespace

#endif  // SDLPP_VIDEO_H
