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
                // Still touching, restart attack timer
                attackStartTime = now;
            } else {
                // Not touching anymore, switch back to moving
                state = State::MOVING;
            }
        }
    }
    else {
        if (isTouching) {
            // Just touched the player, start attacking
            state = State::ATTACKING;
            attackStartTime = now;
            animFrame = 2; // Reset attack animation
        } else {
            // Otherwise move
            moveToward(playerRect, obstacles);
        }
    }

    // Animation
    if (now - lastAnimTime > 100) {
        if (state == State::MOVING) {
            animFrame++;
            if (animFrame > 9) animFrame = 5; // Move frames 5–9
        } else if (state == State::ATTACKING) {
            animFrame++;
            if (animFrame > 4) animFrame = 2; // Attack frames 2–4
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
    SDL_Rect next = rect;

    if (playerRect.x > rect.x) next.x += 2;
    if (playerRect.x < rect.x) next.x -= 2;
    if (playerRect.y > rect.y) next.y += 2;
    if (playerRect.y < rect.y) next.y -= 2;

    if (!willCollide(next, obstacles)) {
        rect = next;
    }
}
