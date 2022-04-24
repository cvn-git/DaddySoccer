#include "displayobject.h"
#include <sdlpp_common.h>

#include <algorithm>
#include <cmath>

DisplayObject::DisplayObject(SdlPP::Renderer &renderer)
    : renderer_(renderer)
{
}

TextureObject::TextureObject(SdlPP::Texture &texture)
    : DisplayObject(texture.renderer())
    , texture_(texture)
{
}

void TextureObject::render()
{
    renderer().copy(texture(), NULL, &displayRect());
}

Goalie::Goalie(SdlPP::Renderer &renderer, const Wall &wall, double xOffset)
    : DisplayObject(renderer)
    , Wall(wall)
    , xOffset_(xOffset)
{
}

void Goalie::resize(const SDL_Rect &fieldRect)
{
    fieldRect_ = fieldRect;
    auto width = fieldRect.w;
    displayWidth_ = int(std::round(std::abs(xOffset_ * width)));
    displayHeight_ = int(std::round(v_.vy_ * width));
    updatePosition();
}

void Goalie::stepPhysics(double currentTime)
{
    p_.y_ += speed_ * (currentTime - lastTime_);
    lastTime_ = currentTime;
    updatePosition();
}

void Goalie::render()
{
    renderer().fillRect(displayRect(), SdlPP::Color(0xA0, 0, 0));
}

void Goalie::updatePosition()
{
    auto width = fieldRect_.w;
    setDisplayRect(SdlPP::createRect(int(std::round(std::min(p_.x_, p_.x_ + xOffset_) * width)) + fieldRect_.x,
                              int(std::round(p_.y_ * width)) + fieldRect_.y,
                              displayWidth_,
                              displayHeight_));
}
