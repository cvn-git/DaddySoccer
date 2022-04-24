#ifndef DADDYS_SOCCER_BOUNCED_OBJECT_H
#define DADDYS_SOCCER_BOUNCED_OBJECT_H

#include "displayobject.h"
#include "geometry.h"

#include <vector>

class BouncedObject : public TextureObject
{
public:
    BouncedObject(SdlPP::Texture &texture, double radius);
    virtual ~BouncedObject() override {}

    double radius() const {return radius_;}
    LogicalPoint position() const {return position_;}
    void setPosition(const LogicalPoint &position) {position_ = position;}
    LogicalVector velocity() const {return velocity_;}
    void setVelocity(const LogicalVector &velocity) {velocity_ = velocity;}

    void setWalls(std::vector<Wall*> &walls) {walls_ = walls;}
    void resetPhysics(const LogicalPoint &position, const LogicalVector&velocity, double currentTime);
    virtual void stepPhysics(double currentTime);
    void resize(const SDL_Rect &fieldRect);

protected:
    void updatePosition();

private:
    double radius_;
    std::vector<Wall*> walls_;
    LogicalPoint position_;
    LogicalVector velocity_;
    SDL_Rect fieldRect_ = {};
    double lastTime_ = 0.0;
};

class Player : public BouncedObject
{
public:
    Player(SdlPP::Texture &texture, double radius);
    ~Player() override {}
};

class Ball : public BouncedObject
{
public:
    Ball(SdlPP::Texture &texture, double radius);
    ~Ball() override {}

    void stepPhysics(double currentTime) override;

    Player* player() const {return player_;}
    void setPlayer(Player *player, double currentTime);

    double lastTackledTime() const {return lastTackledTime_;}

private:
    Player *player_ = nullptr;
    double lastTackledTime_ = 0.0;
};


#endif  // DADDYS_SOCCER_BOUNCED_OBJECT_H
