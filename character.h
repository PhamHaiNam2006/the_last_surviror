#ifndef _CHARACTER_H
#define _CHARACTER_H
#include "movement.h"
#include <SDL.h>

class MainChar {
public:
    MainChar(int health);

    void renderHealthBar(SDL_Renderer* renderer) const ;

    SDL_Rect& getRect() { return rect; }
    int getHealth() const { return health; }
    void reduceHealth(int amount) { health = (health - amount > 0) ? health - amount : 0; }
    void displayMain(SDL_Renderer *renderer, SDL_Rect rect, bool isLeft, bool isUp, bool isDown) ;
private:
    SDL_Rect rect;
    int health;
};
#endif
