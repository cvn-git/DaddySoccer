#include "bouncedobject.h"
#include <sdlpp_common.h>

#include <algorithm>
#include <cmath>

BouncedObject::BouncedObject(SdlPP::Texture &texture, double radius)
    : TextureObject(texture)
    , radius_(radius)
{
}

void BouncedObject::resetPhysics(const LogicalPoint &position, const LogicalVector&velocity, double currentTime)
{
    position_ = position;
    velocity_ = velocity;
    lastTime_ = currentTime;
}

void BouncedObject::stepPhysics(double currentTime)
{
    double dt = currentTime - lastTime_;
    lastTime_ = currentTime;

    LogicalPoint prev(position_);

    LogicalVector v(velocity_.vx_ * dt, velocity_.vy_ * dt);
    if (v.length() < 1e-50)
        return;

    position_.x_ += v.vx_;
    position_.y_ += v.vy_;

    std::vector<bool> wallChecked(walls_.size(), false);
    bool bounced;
    bool changed = false;
    do
    {
        bounced = false;
        for (size_t k = 0; k < walls_.size(); k++)
        {
            auto &wall = walls_[k];
            if ((!wallChecked[k]) && wall->bounce(prev, position_, radius()))
            {
                wallChecked[k] = true;
                bounced = true;
                changed = true;
                break;
            }
        }
    } while (bounced);

    if (changed)
    {
        LogicalLine line(prev, position_);
        double velocity = velocity_.length();
        double scale = velocity / line.v_.length();
        velocity_.vx_ = line.v_.vx_ * scale;
        velocity_.vy_ = line.v_.vy_ * scale;
    }

    updatePosition();
}

void BouncedObject::resize(const SDL_Rect &fieldRect)
{
    fieldRect_ = fieldRect;
    updatePosition();
}

void BouncedObject::updatePosition()
{
    int w = 0, h = 0;
    texture().getSize(w, h);
    double scale = (radius_ * fieldRect_.w) / (std::min(w, h) * 0.5);

    double ww = w * scale;
    double hh = h * scale;
    setDisplayRect(SdlPP::createRect(
                        int(std::round(((position_.x_ * fieldRect_.w) + fieldRect_.x) - (ww * 0.5))),
                        int(std::round(((position_.y_ * fieldRect_.w) + fieldRect_.y) - (hh * 0.5))),
                        int(std::round(ww)),
                        int(std::round(hh))));
}


Player::Player(SdlPP::Texture &texture, double radius)
    : BouncedObject(texture, radius)
{
}

Ball::Ball(SdlPP::Texture &texture, double radius)
    : BouncedObject(texture, radius)
{
}

void Ball::stepPhysics(double currentTime)
{
    if (player() == nullptr)
    {
        BouncedObject::stepPhysics(currentTime);
    }
    else
    {
        auto pos = player()->position();
        auto v = player()->velocity();
        if (v.length() < 1e-40)
        {
            setPosition(pos);
        }
        else
        {
            auto angle = v.angle();
            auto r = player()->radius();
            setPosition(LogicalPoint(pos.x_ + (r * std::cos(angle)), pos.y_ + (r * std::sin(angle))));
        }
        updatePosition();
    }
}

void Ball::setPlayer(Player *player, double currentTime)
{
    player_ = player;
    lastTackledTime_ = currentTime;
}
