#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>
#include "obstacle.h"

class Enemy {
public:
    Enemy(SDL_Texture* tex, int x, int y);
    void update(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles);
    void render(SDL_Renderer* renderer, const SDL_Rect& camera);

private:
    SDL_Rect rect;
    SDL_Texture* texture;

    enum class State {
        MOVING,
        ATTACKING
    } state;

    int animFrame = 0;
    Uint32 lastAnimTime = 0;
    const int frameW = 16, frameH = 15;
    SDL_Rect getAnimSrcRect() const;

    void moveToward(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles);
    bool touchingPlayer(const SDL_Rect& playerRect);
};

#endif
