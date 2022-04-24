#include <sdlpp_joystick.h>

#include <iostream>
#include <exception>
#include <sstream>

namespace SdlPP {

JoystickManager::JoystickManager()
{
    int num = SDL_NumJoysticks();
    std::cout << num << " joystick(s) available" << std::endl;
    for (int idx = 0; idx < num; idx++)
    {
        std::cout << "Opening joystick " << SDL_JoystickNameForIndex(idx) << std::endl;
        auto joy = SDL_JoystickOpen(idx);
        if (joy == NULL)
        {
            throw std::runtime_error("Cannot open joystick");
        }
        allJoysticks_.push_back(joy);
    }
}

JoystickManager::~JoystickManager()
{
    for (auto joy : allJoysticks_)
    {
        if (SDL_JoystickGetAttached(joy))
        {
            std::cout << "Closing a joystick" << std::endl;
            SDL_JoystickClose(joy);
        }
    }
}

JoystickEvent JoystickManager::processEvent(const SDL_Event &event) const
{
    const SDL_JoyButtonEvent &jbutton = event.jbutton;
    const SDL_JoyAxisEvent &jaxis = event.jaxis;
    const SDL_JoyHatEvent &jhat = event.jhat;

    JoystickEvent result;
    std::stringstream ss;

    switch (event.type)
    {
    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
        result.joystickIndex = findJoystickIndex(SDL_JoystickFromInstanceID(jbutton.which));
        result.type = JOYSTICK_BUTTON;
        result.controlIndex = jbutton.button;
        result.value = (jbutton.state == SDL_PRESSED)? 1.0f : 0.0f;
        ss << "button" << result.controlIndex << ((jbutton.state == SDL_PRESSED)? "_pressed" : "_release");
        break;

    case SDL_JOYAXISMOTION:
        result.joystickIndex = findJoystickIndex(SDL_JoystickFromInstanceID(jaxis.which));
        result.type = JOYSTICK_AXIS;
        result.controlIndex = jaxis.axis;
        result.value = jaxis.value / 32768.0f;
        ss << "axis" << result.controlIndex;
        if (result.value > 0.5f)
            ss << "_max";
        else if (result.value < -0.5f)
            ss << "_min";
        else
            ss << "_mid";
        break;

    case SDL_JOYHATMOTION:
        result.joystickIndex = findJoystickIndex(SDL_JoystickFromInstanceID(jhat.which));
        result.type = JOYSTICK_HAT;
        result.controlIndex = jhat.hat;
        ss << "hat" << result.controlIndex;
        switch (jhat.value)
        {
        case SDL_HAT_CENTERED:
            ss << "_centered";
            break;
        case SDL_HAT_UP:
            ss << "_up";
            break;
        case SDL_HAT_DOWN:
            ss << "_down";
            break;
        case SDL_HAT_LEFT:
            ss << "_left";
            break;
        case SDL_HAT_RIGHT:
            ss << "_right";
            break;
        case SDL_HAT_LEFTDOWN:
            ss << "_leftdown";
            break;
        case SDL_HAT_LEFTUP:
            ss << "_leftup";
            break;
        case SDL_HAT_RIGHTDOWN:
            ss << "_rightdown";
            break;
        case SDL_HAT_RIGHTUP:
            ss << "_rightup";
            break;
        }

    default:
        break;
    }

    result.desc = ss.str();
    //std::cout << "Joystick" << result.joystickIndex << ": " << result.desc << std::endl;
    return result;
}

int JoystickManager::findJoystickIndex(SDL_Joystick *joystick) const
{
    for (size_t k = 0; k < allJoysticks_.size(); k++)
    {
        if (joystick == allJoysticks_[k])
            return int(k);
    }
    return -1;
}


}   // SdlPP namespace
