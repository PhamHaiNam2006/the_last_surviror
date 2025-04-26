#include "enemy.h"
#include "movement.h"
#include <cmath>

Enemy::Enemy(SDL_Texture* tex, int x, int y) {
    texture = tex;
    rect = { x, y, 32, 32 };
    state = State::MOVING;
}

void Enemy::update(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    Uint32 now = SDL_GetTicks();

    bool isTouching = touchingPlayer(playerRect);

    if (state == State::ATTACKING) {
        if (now - attackStartTime >= 285) {
            if (isTouching) {
                attackStartTime = now;
            } else {
                state = State::MOVING;
            }
        }
    }
    else {
        if (isTouching) {
            state = State::ATTACKING;
            attackStartTime = now;
            animFrame = 2;
        } else {
            moveToward(playerRect, obstacles);
        }
    }

    if (now - lastAnimTime > 100) {
        if (state == State::MOVING) {
            animFrame++;
            if (animFrame > 9) animFrame = 5;
        } else if (state == State::ATTACKING) {
            animFrame++;
            if (animFrame > 4) animFrame = 2;
        }
        lastAnimTime = now;
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_Rect screenRect = rect;
    SDL_Rect src = getAnimSrcRect();
    SDL_RenderCopy(renderer, texture, &src, &screenRect);
}


SDL_Rect Enemy::getAnimSrcRect() const {
    return SDL_Rect{ animFrame * frameW, 0, frameW, frameH };
}

bool Enemy::touchingPlayer(const SDL_Rect& playerRect) {
    return SDL_HasIntersection(&rect, &playerRect);
}

void Enemy::moveToward(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    int dx = 0;
    int dy = 0;

    int diffX = playerRect.x - rect.x;
    int diffY = playerRect.y - rect.y;

    if (diffX < 0) dx = -2;
    else if (diffX > 0) dx = 2;

    if (diffY < 0) dy = -2;
    else if (diffY > 0) dy = 2;

    SDL_Rect nextX = rect;
    nextX.x += dx;
    if (!willCollide(nextX, obstacles)) {
        rect.x = nextX.x;
    }

    SDL_Rect nextY = rect;
    nextY.y += dy;
    if (!willCollide(nextY, obstacles)) {
        rect.y = nextY.y;
    }
}

