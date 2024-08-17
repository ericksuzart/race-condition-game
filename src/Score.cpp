#include "../include/Score.hpp"
using namespace std;

Score::Score(std::vector<Robot *> const robots)
: robots(robots), scoreRobot1(0), scoreRobot2(0)
{
}

void Score::reset()
{
    for (auto robot : robots) robot->reset();
};

void Score::updateScore()
{

};

int Score::getScoreRobot1() { return scoreRobot1; }
int Score::getScoreRobot2() { return scoreRobot2; }
