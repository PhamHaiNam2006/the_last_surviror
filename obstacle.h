#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
public:
    Obstacle(int x, int y, int w, int h);
    void render(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;

private:
    SDL_Rect rect;
};

#endif
