#include <sdlpp_video.h>

#include <stdexcept>

namespace SdlPP {

//------------------------------------------------------------------------------
Window::Window(const std::string &title, int width, int height, Uint32 windowFlags, Uint32 rendererFlags)
{
    window_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, windowFlags);
    if (window_ == NULL)
        throw std::runtime_error("Cannot create SDL window");

    renderer_.reset(new Renderer(*this, rendererFlags));
}

Window::~Window()
{
    if (window_ != nullptr)
        SDL_DestroyWindow(window_);
}

void Window::getSize(int &width, int &height) const
{
    SDL_GetWindowSize(window_, &width, &height);
}

SDL_Rect Window::getSize() const
{
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    getSize(rect.w, rect.h);
    return rect;
}


}   // SdlPP namespace
