#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
public:
    Obstacle(int x, int y, int w, int h, bool it);
    void render(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;
    void health_render(SDL_Renderer* renderer) const;

private:
    SDL_Rect rect;
    SDL_Rect rect2;
};

#endif
