#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>
#include <SDL_image.h>

class Obstacle {
public:
    Obstacle(int x,int y, int w,int h, int type);
    void render(SDL_Renderer* renderer, SDL_Texture* texture) const;
    SDL_Rect getRect() const {return rect;}
    int getType() const {return type;}
    SDL_Rect getTileClip() const;

private:
    SDL_Rect rect;
    int type;
};

#endif
