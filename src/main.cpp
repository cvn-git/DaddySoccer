#include "mainwindow.h"

#include <iostream>
#include <stdexcept>

class App
{
public:
    App()
        : running_(false)
    {
    }

    void run()
    {
        mainWindow_.resize();

        running_ = true;

        SDL_Event ev;
        while (running_)
        {
            // Physics
            mainWindow_.stepPhysics(SdlPP::getCurrentTime());

            // Render
            mainWindow_.render();

            // Event loop
            while (SDL_PollEvent(&ev) != 0)
            {
                // check event type
                switch (ev.type)
                {
                case SDL_QUIT:
                    // shut down
                    running_ = false;
                    break;
                case SDL_KEYDOWN:
                    // test keycode
                    switch ( ev.key.keysym.sym )
                    {
                    case SDLK_ESCAPE:
                        running_ = false;
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_JOYBUTTONDOWN:
                case SDL_JOYBUTTONUP:
                case SDL_JOYAXISMOTION:
                case SDL_JOYHATMOTION:
                    mainWindow_.processJoystickEvent(joystickManager_.processEvent(ev));
                    break;
                case SDL_WINDOWEVENT:
                    switch (ev.window.event)
                    {
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                    case SDL_WINDOWEVENT_RESIZED:
                        mainWindow_.resize();
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }

private:
    MainWindow mainWindow_;
    SdlPP::JoystickManager joystickManager_;

    bool running_;
};


#undef main // undefine main in SDL
int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0)
    {
        fprintf(stderr, "Could not intialize SDL2_ttf: %s\n", TTF_GetError());
        return 2;
    }

    try
    {
        App app;
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cout << "EXCEPTION: " << e.what() << std::endl;
    }

    TTF_Quit();
    SDL_Quit();
    return 0;
}
