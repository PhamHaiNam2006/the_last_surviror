#include "enemy.h"
#include "movement.h"
#include <cmath>

Enemy::Enemy(SDL_Texture* tex, int x, int y) {
    texture = tex;
    rect = { x, y, 32, 32 };
    state = State::MOVING;
}

SDL_Rect Enemy::getAttackBox() const {
    return attackBox;
}

void Enemy::update(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    Uint32 now = SDL_GetTicks();
    bool isTouching = touchingPlayer(playerRect);
    facingLeft = (playerRect.x < rect.x);

    if (isTouching) {
        if (state != State::ATTACKING) {
            state = State::ATTACKING;
            attackStartTime = now;
            animFrame = 2;
            if (facingLeft) {
        attackBox = { rect.x - 32, rect.y, 32, 32 };
    } else {
        attackBox = { rect.x + rect.w, rect.y, 32, 32 };
    }
        } else if (now - attackStartTime >= 500) {
            attackStartTime = now;
            animFrame = 2;
        }
    } else {
        state = State::MOVING;
        moveToward(playerRect, obstacles);
        attackBox = {0, 0, 0, 0};
    }

    if (playerRect.x < rect.x) {
        facingLeft = true;
    } else if (playerRect.x > rect.x) {
        facingLeft = false;
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

    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, texture, &src, &screenRect, 0, nullptr, flip);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect drawBox = { attackBox.x, attackBox.y, attackBox.w, attackBox.h };
    SDL_RenderDrawRect(renderer, &drawBox);
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

