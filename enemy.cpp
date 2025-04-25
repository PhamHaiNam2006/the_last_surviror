#include "enemy.h"
#include "movement.h"
#include <cmath>

Enemy::Enemy(SDL_Texture* tex, int x, int y) {
    texture = tex;
    rect = { x, y, 32, 32 };
    state = State::MOVING;
}

void Enemy::update(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    bool wasTouching = touchingPlayer(playerRect);

    if (wasTouching) {
        state = State::ATTACKING;
    } else {
        state = State::MOVING;
        moveToward(playerRect, obstacles); // Continue moving toward the player
    }

    // Update animation
    Uint32 now = SDL_GetTicks();
    if (now - lastAnimTime > 100) {
        if (state == State::MOVING) {
            // If moving, cycle through frames 5-9
            animFrame = (animFrame + 1) % 5 + 5;
        } else if (state == State::ATTACKING) {
            // If attacking, cycle through frames 2-4
            animFrame = (animFrame + 1) % 3 + 2;
        }

        lastAnimTime = now;
    }

    // Handle animation frame reset when state changes (avoid glitch)
    if (state == State::MOVING && wasTouching) {
        animFrame = 5;  // Start the moving animation from the beginning if transitioning from attack to move
    } else if (state == State::ATTACKING && !wasTouching) {
        animFrame = 2;  // Start the attacking animation from the beginning if transitioning from move to attack
    }
}

void Enemy::render(SDL_Renderer* renderer, const SDL_Rect& camera) {
    SDL_Rect screenRect = rect;
    screenRect.x -= camera.x;
    screenRect.y -= camera.y;

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
