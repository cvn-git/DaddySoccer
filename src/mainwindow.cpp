#include "mainwindow.h"

#include <cmath>
#include <sstream>
#include <iostream>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

static const int numBalls = 1;

static const double goalDepth = 0.1;
static const double goalWidth = 0.2;
static const double fieldRatio = 1.8;
static const double fieldTopGap = 0.1;
static const double fieldBottomGap = 0.02;

static const double playerSpeed = 0.2;
static const double goalieSpeed = 0.12;
static const double ballPassSpeed = 0.3;
static const double ballShootSpeed = 0.5;

static const double ballRadius = 0.01;
static const double playerRadius = 0.02;

static const double goalieWidth = 0.04;
static const double goalieDepth = 0.01;

static const double minTacklingTime = (4 * playerRadius) / playerSpeed;

static const std::string dataPath = "../DaddySoccer/data";
static const std::string scoreFont = dataPath + "/OpenSans-Bold.ttf";

std::random_device randomDevice;

MainWindow::MainWindow()
    : Window("Daddy's Soccer", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED)
    , team1Texture_(renderer(), dataPath + "/capam.bmp")
    , team2Texture_(renderer(), dataPath + "/radiation.bmp")
    , ballTexture_(renderer(), dataPath + "/ball.bmp")
    , fieldTile_(dataPath + "/grass.bmp")
    , fieldRect_{}
    , randomEngine_(randomDevice())
    , scores_(2, 0)
    , displayedScores_(2, -1)
{
    // Joystick commands
    joystickCommands_["axis1_min"] = PLAYER1_UP;
    joystickCommands_["axis1_max"] = PLAYER1_DOWN;
    joystickCommands_["axis0_min"] = PLAYER1_LEFT;
    joystickCommands_["axis0_max"] = PLAYER1_RIGHT;
    joystickCommands_["button0_pressed"] = PLAYER2_UP;
    joystickCommands_["button2_pressed"] = PLAYER2_DOWN;
    joystickCommands_["button3_pressed"] = PLAYER2_LEFT;
    joystickCommands_["button1_pressed"] = PLAYER2_RIGHT;
    joystickCommands_["button6_pressed"] = GOALIE_UP;
    joystickCommands_["button4_pressed"] = GOALIE_DOWN;
    joystickCommands_["button6_release"] = GOALIE_STAY;
    joystickCommands_["button4_release"] = GOALIE_STAY;
    joystickCommands_["button7_pressed"] = SHOOT;
    joystickCommands_["button5_pressed"] = PASS;

    // Create game
    allPlayers_.emplace_back(new Player(team1Texture_, playerRadius));
    allPlayers_.emplace_back(new Player(team1Texture_, playerRadius));
    allPlayers_.emplace_back(new Player(team2Texture_, playerRadius));
    allPlayers_.emplace_back(new Player(team2Texture_, playerRadius));

    for (int k = 0; k < numBalls; k++)
        allBalls_.emplace_back(new Ball(ballTexture_, ballRadius));

    allGoalies_.emplace_back(new Goalie(renderer(), Wall(LogicalPoint(0.0, 0.5 * (1 / fieldRatio - goalieWidth)), LogicalVector(0.0, goalieWidth)), -goalieDepth));
    allGoalies_.emplace_back(new Goalie(renderer(), Wall(LogicalPoint(1.0, 0.5 * (1 / fieldRatio - goalieWidth)), LogicalVector(0.0, goalieWidth)), goalieDepth));

    for (auto &player : allPlayers_)
        allDisplayObjects_.push_back(player.get());

    for (auto &ball : allBalls_)
        allDisplayObjects_.push_back(ball.get());

    for (auto &goalie : allGoalies_)
        allDisplayObjects_.push_back(goalie.get());

    double h = 1.0 / fieldRatio;
    playerWalls_.emplace_back(LogicalPoint(0.0, 0.0), LogicalVector(1.0, 0.0));
    playerWalls_.emplace_back(LogicalPoint(1.0, 0.0), LogicalVector(0.0, h));
    playerWalls_.emplace_back(LogicalPoint(1.0, h), LogicalVector(-1.0, 0.0));
    playerWalls_.emplace_back(LogicalPoint(0.0, h), LogicalVector(0.0, -h));
    std::vector<Wall*> playerWallHandles;
    for (auto &wall : playerWalls_)
        playerWallHandles.push_back(&wall);

    for (auto &player : allPlayers_)
        player->setWalls(playerWallHandles);

    double offset = (h - goalWidth) * 0.5;
    ballWalls_.emplace_back(LogicalPoint(0.0, 0.0), LogicalVector(1.0, 0.0));
    ballWalls_.emplace_back(LogicalPoint(1.0, 0.0), LogicalVector(0.0, offset));
    ballWalls_.emplace_back(LogicalPoint(1.0, h), LogicalVector(0.0, -offset));
    ballWalls_.emplace_back(LogicalPoint(1.0, h), LogicalVector(-1.0, 0.0));
    ballWalls_.emplace_back(LogicalPoint(0.0, h), LogicalVector(0.0, -offset));
    ballWalls_.emplace_back(LogicalPoint(0.0, 0.0), LogicalVector(0.0, offset));
    std::vector<Wall*> ballWallHandles;
    for (auto &wall : ballWalls_)
        ballWallHandles.push_back(&wall);
    for (auto &goalie : allGoalies_)
        ballWallHandles.push_back(goalie.get());

    for (auto &ball : allBalls_)
        ball->setWalls(ballWallHandles);

    // Initial positions
    allPlayers_[0]->resetPhysics(LogicalPoint(0.1, 0.05), LogicalVector(0.0, 0.0), 0.0);
    allPlayers_[1]->resetPhysics(LogicalPoint(0.1, h - 0.05), LogicalVector(0.0, 0.0), 0.0);
    allPlayers_[2]->resetPhysics(LogicalPoint(0.9, 0.05), LogicalVector(0.0, 0.0), 0.0);
    allPlayers_[3]->resetPhysics(LogicalPoint(0.9, h - 0.05), LogicalVector(0.0, 0.0), 0.0);

#if 1
    for (auto &ball : allBalls_)
    {
        newBall(ball.get());
    }
#else
    allBalls_[0]->resetPhysics(LogicalPoint(0.3, 0.3), LogicalVector(-0.4, -0.4), 0.0);
#endif
}

MainWindow::~MainWindow()
{
    if (numRenderingFrames_ > 0)
    {
        double sec = SDL_GetTicks() * 1e-3;
        std::cout << "Running time: " << sec << " sec" << std::endl;
        std::cout << "Average rendering speed: " << numRenderingFrames_ / sec << " frames/sec" << std::endl;
    }
}

void MainWindow::resizeBackground()
{
    int width = 0;
    int height = 0;
    getSize(width, height);

    // Resize field
    double fieldWidth = width / (1.0 + 2.0 * goalDepth);
    double topGap = fieldWidth * fieldTopGap;
    double bottomGap = fieldWidth * fieldBottomGap;
    double fieldHeight = fieldWidth / fieldRatio;
    double ratio = height / (fieldHeight + topGap + bottomGap);
    if (ratio < 1.0)
    {
        fieldHeight *= ratio;
        fieldWidth *= ratio;
        topGap *= ratio;
    }
    double cx = width * 0.5;
    double cy = topGap + (fieldHeight * 0.5);
    fieldRect_.x = int(std::round(cx - (fieldWidth * 0.5)));
    fieldRect_.y = int(std::round(cy - (fieldHeight * 0.5)));
    fieldRect_.w = int(std::round(fieldWidth));
    fieldRect_.h = int(std::round(fieldHeight));

    // Paint background
    SdlPP::Surface surface(width, height);
    SdlPP::Renderer renderer(surface);
    renderer.clear(SdlPP::Color(0x7F, 0x7F, 0x7F));
    SdlPP::Texture textureTile(renderer, fieldTile_);
    renderer.tile(textureTile, fieldRect_);
    renderer.fillRect(SdlPP::createRect(int(std::round(cx - (fieldWidth * (0.5 + goalDepth)))),
                                 int(std::round(cy - (fieldWidth * goalWidth * 0.5))),
                                 int(std::round(fieldWidth * goalDepth)),
                                 int(std::round(fieldWidth * goalWidth))),
                                 SdlPP::Color(0x00, 0xA0, 0x00));
    renderer.fillRect(SdlPP::createRect(int(std::round(cx + (fieldWidth * 0.5))),
                                 int(std::round(cy - (fieldWidth * goalWidth * 0.5))),
                                 int(std::round(fieldWidth * goalDepth)),
                                 int(std::round(fieldWidth * goalWidth))),
                                 SdlPP::Color(0x00, 0xA0, 0x00));

    textureBackground_.reset(new SdlPP::Texture(this->renderer(), surface));
}

void MainWindow::updateScoreDisplay()
{
    int width = 0;
    int height = 0;
    getSize(width, height);
    double fieldWidth = width / (1.0 + 2.0 * goalDepth);

    SdlPP::Font font(scoreFont, int(std::round(fieldWidth * fieldTopGap * 0.8)));
    std::stringstream ss;
    ss << scores_[0] << " - " << scores_[1];
    SdlPP::Surface surface(font, ss.str(), SdlPP::Color(0xFF, 0xFF, 0xFF));
    int scoreWidth = 0, scoreHeight = 0;
    surface.getSize(scoreWidth, scoreHeight);
    scoreRect_ = SdlPP::createRect((width - scoreWidth) / 2, 0, scoreWidth, scoreHeight);
    textureScore_.reset(new SdlPP::Texture(this->renderer(), surface));
    displayedScores_[0] = scores_[0];
    displayedScores_[1] = scores_[1];
}

void MainWindow::resize()
{
    resizeBackground();
    updateScoreDisplay();

    for (auto &player : allPlayers_)
        player->resize(fieldRect_);

    for (auto &ball : allBalls_)
        ball->resize(fieldRect_);

    for (auto &goalie : allGoalies_)
        goalie->resize(fieldRect_);
}

void MainWindow::renderBackground()
{
    renderer().copy(*textureBackground_, NULL, NULL);
}

void MainWindow::renderScoreDisplay()
{
    renderer().copy(*textureScore_, NULL, &scoreRect_);
}

void MainWindow::render()
{
    renderBackground();

    if ((displayedScores_[0] != scores_[0]) || (displayedScores_[1] != scores_[1]))
    {
        updateScoreDisplay();
    }
    renderScoreDisplay();


    for (auto &obj : allDisplayObjects_)
        obj->render();

    renderer().present();
    ++numRenderingFrames_;
}

void MainWindow::stepPhysics(double time)
{
    for (auto &goalie : allGoalies_)
        goalie->stepPhysics(time);

    for (auto &player : allPlayers_)
        player->stepPhysics(time);

    for (auto &ball : allBalls_)
        ball->stepPhysics(time);

    // Check tackling
    for (auto &ball : allBalls_)
    {
        if (time < (ball->lastTackledTime() + minTacklingTime))
            continue;

        for (auto &player : allPlayers_)
        {
            if ((ball->player() != player.get()) && (LogicalVector(ball->position(), player->position()).length() < player->radius()))
            {
                ball->setPlayer(player.get(), time);
                break;
            }
        }
    }

    // Check score
    for (auto &ball : allBalls_)
    {
        auto pos = ball->position();
        if (pos.x_ < -0.1)
            scores_[1] += 1;
        else if (pos.x_ > 1.1)
            scores_[0] += 1;
        else
            continue;

        newBall(ball.get());
    }
}

void MainWindow::processJoystickEvent(const SdlPP::JoystickEvent &event)
{
    auto teamIdx = event.joystickIndex;
    if ((teamIdx < 0) || (teamIdx > 2))
        return;

    auto it = joystickCommands_.find(event.desc);
    if (it == joystickCommands_.end())
        return;
    else
        processUserCommand(teamIdx, it->second);
}

void MainWindow::processUserCommand(int teamIdx, UserCommand command)
{
    switch (command)
    {
    case PLAYER1_UP:
        allPlayers_[teamIdx * 2 + 0]->setVelocity(LogicalVector(0.0, -playerSpeed));
        break;
    case PLAYER1_DOWN:
        allPlayers_[teamIdx * 2 + 0]->setVelocity(LogicalVector(0.0, playerSpeed));
        break;
    case PLAYER1_LEFT:
        allPlayers_[teamIdx * 2 + 0]->setVelocity(LogicalVector(-playerSpeed, 0.0));
        break;
    case PLAYER1_RIGHT:
        allPlayers_[teamIdx * 2 + 0]->setVelocity(LogicalVector(playerSpeed, 0.0));
        break;
    case PLAYER2_UP:
        allPlayers_[teamIdx * 2 + 1]->setVelocity(LogicalVector(0.0, -playerSpeed));
        break;
    case PLAYER2_DOWN:
        allPlayers_[teamIdx * 2 + 1]->setVelocity(LogicalVector(0.0, playerSpeed));
        break;
    case PLAYER2_LEFT:
        allPlayers_[teamIdx * 2 + 1]->setVelocity(LogicalVector(-playerSpeed, 0.0));
        break;
    case PLAYER2_RIGHT:
        allPlayers_[teamIdx * 2 + 1]->setVelocity(LogicalVector(playerSpeed, 0.0));
        break;
    case GOALIE_UP:
        allGoalies_[teamIdx]->setSpeed(-goalieSpeed);
        break;
    case GOALIE_DOWN:
        allGoalies_[teamIdx]->setSpeed(goalieSpeed);
        break;
    case GOALIE_STAY:
        allGoalies_[teamIdx]->setSpeed(0.0);
        break;
    case SHOOT:
        shoot(teamIdx);
        break;
    case PASS:
        pass(teamIdx);
        break;
    default:
        break;
    }
}

void MainWindow::newBall(Ball *ball)
{
    std::uniform_real_distribution<double> dist1(0.35, 0.65);
    std::uniform_real_distribution<double> dist2(ballRadius * 2, 1.0 / fieldRatio - ballRadius * 2);
    ball->setPosition(LogicalPoint(dist1(randomEngine_), dist2(randomEngine_)));

    auto dir = dist1(randomEngine_);
    double speed = (dir > 0.5)? ballPassSpeed : -ballPassSpeed;
    ball->setVelocity(LogicalVector(0.0, speed));
}

void MainWindow::pass(int teamIdx)
{
    for (auto &ball : allBalls_)
    {
        auto player = ball->player();
        for (int idx = 0; idx < 2; idx++)
        {
            if (player == allPlayers_[teamIdx * 2 + idx].get())
            {
                auto other = allPlayers_[teamIdx * 2 + 1 - idx].get();
                ball->setPosition(player->position());
                auto va = other->velocity();
                double vA = va.length();
                double angle = 0.0;
                if (vA > 1e-20)
                {
                    LogicalVector ab(other->position(), ball->position());
                    double AB = ab.length();
                    double k = ballPassSpeed / playerSpeed;
                    double cosA = ab.dotProduct(other->velocity()) / (AB * vA);
                    double ABcosA = AB * cosA;
                    double delta = ABcosA * ABcosA + (k * k - 1.0) * AB * AB;
                    double AC = (std::sqrt(delta) - ABcosA) / (k * k - 1.0);
                    LogicalVector bc(va.vx_ * AC / vA - ab.vx_, va.vy_ * AC / vA - ab.vy_);
                    angle = bc.angle();
                }
                else
                {
                    angle = LogicalVector(ball->position(), other->position()).angle();
                }
                double curTime = SdlPP::getCurrentTime();
                ball->setPlayer(nullptr, curTime);
                ball->resetPhysics(ball->position(), LogicalVector(ballPassSpeed * std::cos(angle), ballPassSpeed * std::sin(angle)), curTime);
            }
        }
    }
}

void MainWindow::shoot(int teamIdx)
{
    double offset = (1.0 / fieldRatio - goalWidth) * 0.5;
    std::uniform_real_distribution<double> dist(offset, offset + goalWidth);
    for (auto &ball : allBalls_)
    {
        auto player = ball->player();
        for (int idx = 0; idx < 2; idx++)
        {
            if (player == allPlayers_[teamIdx * 2 + idx].get())
            {
                LogicalPoint target;
                target.x_ = (1 - teamIdx) * 1.0;
                target.y_ = dist(randomEngine_);
                LogicalVector v(player->position(), target);
                double scale = ballShootSpeed / v.length();
                v.vx_ *= scale;
                v.vy_ *= scale;
                double curTime = SdlPP::getCurrentTime();
                ball->setPlayer(nullptr, curTime);
                ball->resetPhysics(player->position(), v, curTime);
            }
        }
    }
}
