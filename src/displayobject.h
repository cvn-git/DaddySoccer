#ifndef DADDYS_SOCCER_DISPLAY_OBJECT_H
#define DADDYS_SOCCER_DISPLAY_OBJECT_H

#include <sdlpp_renderer.h>
#include "geometry.h"


class DisplayObject
{
public:
    DisplayObject(SdlPP::Renderer &renderer);
    virtual ~DisplayObject() {}

    SdlPP::Renderer& renderer() {return renderer_;}

    const SDL_Rect& displayRect() const {return displayRect_;}
    void setDisplayRect(const SDL_Rect &rect) {displayRect_ = rect;}
    virtual void render() = 0;

private:
    SdlPP::Renderer &renderer_;
    SDL_Rect displayRect_ = {};
};

class TextureObject : public DisplayObject
{
public:
    TextureObject(SdlPP::Texture &texture);
    virtual ~TextureObject() {}
    void render() override;

    SdlPP::Texture& texture() const {return texture_;}

private:
    SdlPP::Texture &texture_;
};


class Goalie : public DisplayObject, public Wall
{
public:
    Goalie(SdlPP::Renderer &renderer, const Wall &wall, double xOffset);
    void resize(const SDL_Rect &fieldRect);

    void setSpeed(double speed) {speed_ = speed;}
    void stepPhysics(double currentTime);
    void render() override;

private:
    void updatePosition();

    SDL_Rect fieldRect_ = {};
    double xOffset_;
    double speed_ = 0.0;
    double lastTime_ = 0.0;
    int displayWidth_ = 0;
    int displayHeight_ = 0;
};

#endif  // DADDYS_SOCCER_DISPLAY_OBJECT_H
