#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SDL.h>
#include <vector>
#include "obstacle.h"

class Movement {
public:
    Movement(int speed);
    void handleEvent(const SDL_Event& event);
    void update(SDL_Rect& rect, const std::vector<Obstacle>& obstacles, int &n,int &amount);
private:
    int speed;
    int dx, dy;
};

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);

#endif
