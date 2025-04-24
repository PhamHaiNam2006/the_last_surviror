#include "enemy.h"
#include <cmath>

Enemy::Enemy(SDL_Texture* tex, int x, int y)
    : texture(tex), rect{ x, y, 32, 32 }, frame(0), frameCounter(0),
      lastFrameTime(0), frameDelay(150), actionDelay(200), lastActionTime(0),
      state(EnemyState::IDLE), active(false) {
    hitbox = { x + 4, y + 4, 24, 24 }; // Slightly smaller hitbox
}

void Enemy::activate() {
    active = true;
}

bool Enemy::isActive() const {
    return active;
}

SDL_Rect Enemy::getHitbox() const {
    return hitbox;
}

bool Enemy::isCollidingWithPlayer(const SDL_Rect& player) const {
    return SDL_HasIntersection(&hitbox, &player);
}

void Enemy::update(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& obstacles, bool playerMoved) {
    Uint32 now = SDL_GetTicks();

    if (!active && !playerMoved) return;
    if (playerMoved) active = true;

    if (now - lastActionTime < actionDelay) return;

    lastActionTime = now;

    if (isCollidingWithPlayer(playerRect)) {
        state = EnemyState::ATTACK;
        frame = 2;
    } else if (canSeePlayer(playerRect, obstacles)) {
        state = EnemyState::MOVE;
        moveTowards(playerRect);
        frame = 5;
    } else {
        state = EnemyState::IDLE;
        frame = 0;
    }

    hitbox.x = rect.x + 4;
    hitbox.y = rect.y + 4;
}

void Enemy::animate() {
    switch (state) {
        case EnemyState::IDLE:
            if (frame > 1) frame = 0;
            break;
        case EnemyState::ATTACK:
            if (frame < 2 || frame > 4) frame = 2;
            break;
        case EnemyState::MOVE:
            if (frame < 5 || frame > 9) frame = 5;
            break;
        case EnemyState::DIE:
            if (frame < 10 || frame > 14) frame = 10;
            break;
    }
}

void Enemy::moveTowards(const SDL_Rect& target) {
    int dx = target.x - rect.x;
    int dy = target.y - rect.y;

    if (std::abs(dx) > std::abs(dy)) {
        rect.x += (dx > 0) ? 4 : -4;
    } else {
        rect.y += (dy > 0) ? 4 : -4;
    }
}

bool Enemy::canSeePlayer(const SDL_Rect& player, const std::vector<SDL_Rect>& obstacles) const {
    SDL_Rect vision = { std::min(player.x, rect.x), std::min(player.y, rect.y),
                        std::abs(player.x - rect.x), std::abs(player.y - rect.y) };

    for (const auto& obs : obstacles) {
        if (SDL_HasIntersection(&vision, &obs)) return false;
    }
    return true;
}

SDL_Rect Enemy::getSpriteClip() const {
    int frameW = 14, frameH = 16;
    return { frame * frameW, 0, frameW, frameH };
}

void Enemy::render(SDL_Renderer* renderer, const SDL_Rect& camera) {
    SDL_Rect src = getSpriteClip();
    SDL_Rect dst = { rect.x - camera.x, rect.y - camera.y, rect.w, rect.h };
    SDL_RenderCopy(renderer, texture, &src, &dst);
}
