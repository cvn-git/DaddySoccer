#ifndef DADDYS_SOCCER_MAIN_WINDOW_H
#define DADDYS_SOCCER_MAIN_WINDOW_H

#include <sdlpp.h>

#include "bouncedobject.h"

#include <map>
#include <random>

typedef enum
{
    PLAYER1_UP,
    PLAYER1_DOWN,
    PLAYER1_LEFT,
    PLAYER1_RIGHT,
    PLAYER2_UP,
    PLAYER2_DOWN,
    PLAYER2_LEFT,
    PLAYER2_RIGHT,
    GOALIE_UP,
    GOALIE_DOWN,
    GOALIE_STAY,
    SHOOT,
    PASS
} UserCommand;

class MainWindow : public SdlPP::Window
{
public:
    MainWindow();
    ~MainWindow();

    void resize();
    void render();
    void stepPhysics(double time);
    void processJoystickEvent(const SdlPP::JoystickEvent &event);

private:
    void resizeBackground();
    void renderBackground();
    void updateScoreDisplay();
    void renderScoreDisplay();
    void processUserCommand(int teamIdx, UserCommand command);

    void newBall(Ball *ball);
    void pass(int teamIdx);
    void shoot(int teamIdx);

    SdlPP::Texture team1Texture_;
    SdlPP::Texture team2Texture_;
    SdlPP::Texture ballTexture_;

    SdlPP::Surface fieldTile_;
    std::unique_ptr<SdlPP::Texture> textureBackground_;
    std::unique_ptr<SdlPP::Texture> textureScore_;
    SDL_Rect fieldRect_;
    SDL_Rect scoreRect_;
    Uint32 numRenderingFrames_ = 0;

    std::map<std::string, UserCommand> joystickCommands_;

    std::vector<std::unique_ptr<Player> > allPlayers_;
    std::vector<std::unique_ptr<Ball> > allBalls_;
    std::vector<DisplayObject*> allDisplayObjects_;
    std::vector<std::unique_ptr<Goalie> > allGoalies_;
    std::vector<Wall> playerWalls_;
    std::vector<Wall> ballWalls_;
    std::vector<int> scores_;
    std::vector<int> displayedScores_;

    std::default_random_engine randomEngine_;
};

#endif  // DADDYS_SOCCER_MAIN_WINDOW_H
