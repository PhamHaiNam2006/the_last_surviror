#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
public:
    Obstacle(int x, int y, int w, int h, bool it);
    void render(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;
    void health_render(SDL_Renderer* renderer) const;
    bool redcube() const {
        return obs;
    }
private:
    SDL_Rect rect;
    bool obs;
};

#endif
