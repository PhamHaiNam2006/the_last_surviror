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
    void reduceHealth(int &amount) ;
    void noHealth(bool &running){
        if(health==0){
            running=false;
        }
    }

    void displayMain(SDL_Renderer *renderer, SDL_Rect rect, bool isLeft, bool isUp, bool isDown);

    int width, height;
    bool facingRight;

    bool showSlash;
    Uint32 slashStartTime;
    SDL_Texture* swordTexture;

    void render(SDL_Renderer* renderer);
    void renderSlash(SDL_Renderer* renderer);
    void triggerSlash(int moving);
    void getpos(int &dx,int &dy);
private:
    Uint32 lastDamageTime;
    Uint32 damageCooldown = 1000;
    int x, y;
    SDL_Rect rect;
    int health;
};
#endif
