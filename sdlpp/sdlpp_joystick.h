#ifndef SDLPP_JOYSTICK_H
#define SDLPP_JOYSTICK_H

#include <SDL.h>
#include <SDL_joystick.h>

#include <vector>
#include <string>

namespace SdlPP {

typedef enum
{
    JOYSTICK_NONE,
    JOYSTICK_HAT,
    JOYSTICK_AXIS,
    JOYSTICK_BUTTON
} JoystickControlType;

struct JoystickEvent
{
    int                 joystickIndex   = -1;
    JoystickControlType type            = JOYSTICK_NONE;
    unsigned int        controlIndex    = 0;
    float               value           = 0.0f;
    std::string         desc;
};

class JoystickManager
{
public:
    JoystickManager();
    JoystickManager(const JoystickManager &other) = delete;
    ~JoystickManager();

    JoystickManager& operator=(const JoystickManager &other) = delete;

    JoystickEvent processEvent(const SDL_Event &event) const;

private:
    int findJoystickIndex(SDL_Joystick *joystick) const;

    std::vector<SDL_Joystick*> allJoysticks_;
};


}   // SdlPP namespace

#endif  // SDLPP_JOYSTICK_H
