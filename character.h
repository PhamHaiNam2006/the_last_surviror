#ifndef _CHARACTER_H
#define _CHARACTER_H
#include "movement.h"
#include <SDL.h>

class Character
{
public:
    Character();
    ~Character();
    void move(int x, int y);
    void render();
    void update();
    void handleEvent(SDL_Event& e);
    void setCamera(SDL_Rect& camera);
    SDL_Rect getRect();
};

#endif
