#include "enemy.h"
#include "movement.h"
#include <cmath>
#include <cstdlib>
#include <random>

Enemy::Enemy(SDL_Texture* tex, int x, int y, int heal) {
    texture = tex;
    rect = { x, y, 32, 32 };
    state = State::MOVING;
    health = heal;
}

SDL_Rect Enemy::getAttackBox() const {
    return attackBox;
}

bool Enemy::collidesWithOtherEnemies(const SDL_Rect& rect, const std::vector<Enemy>& others) {
    for (const auto& e : others) {
        if (&e == this) continue;
        if (SDL_HasIntersection(&rect, &e.rect)) {
            return true;
        }
    }
    return false;
}

void Enemy::update(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    if (health == 0) {
        alive = false;
    }
    if (!alive) return;

    Uint32 now = SDL_GetTicks();
    bool isTouching = touchingPlayer(playerRect);
    facingLeft = (playerRect.x < rect.x);

    if (isTouching) {
        if (state != State::ATTACKING) {
            state = State::ATTACKING;
            attackStartTime = now;
            animFrame = 2;

        } else if (now - attackStartTime >= 500) {
            attackStartTime = now;
            animFrame = 2;
        }
    } else {
        state = State::MOVING;
        moveToward(playerRect, obstacles);
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
    if (!alive) return;
    SDL_Rect screenRect = rect;
    SDL_Rect src = getAnimSrcRect();

    SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(renderer, texture, &src, &screenRect, 0, nullptr, flip);

    int barWidth = 30;
    int barHeight = 4;
    int barX = rect.x + (rect.w - barWidth) / 2;
    int barY = rect.y - 10;
    int healthBarWidth = (health > 0) ? (barWidth * health / 100) : 0;

    SDL_Rect back = { barX, barY, barWidth, barHeight };
    SDL_Rect fill = { barX, barY, healthBarWidth, barHeight };

    SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
    SDL_RenderFillRect(renderer, &back);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &fill);
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
    SDL_Rect nextY = rect;
    nextX.x += dx;
    nextY.y += dy;
    if (!willCollide(nextX, obstacles)&&!collidesWithOtherEnemies(nextX, *allEnemies)) {
        rect.x = nextX.x;
    }

    if (!willCollide(nextY, obstacles)&&!collidesWithOtherEnemies(nextY, *allEnemies)) {
        rect.y = nextY.y;
    }
}

bool collidesWithObstacles(const SDL_Rect& rect, const std::vector<Obstacle>& obstacles) {
    for (const auto& obs : obstacles) {
        int type = obs.getType();
        SDL_Rect r = obs.getRect();
        if (SDL_HasIntersection(&rect, &r)&&type!=0) {
            return true;
        }
    }
    return false;
}

void spawnEnemies(std::vector<Enemy>& enemies, SDL_Texture* enemyTexture, int count, int mapWidth, int mapHeight, const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles, float health) {
    enemies.clear();
    const int TILE_SIZE = 32;
    const int MAX_ATTEMPTS = 100;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDist(800, 1000 - TILE_SIZE);
    std::uniform_int_distribution<> yDist(16, 584 - TILE_SIZE);

    int spawned = 0;
    while (spawned < count) {
        int attempts = 0;
        while (attempts < MAX_ATTEMPTS) {
            int x = xDist(gen);
            int y = yDist(gen);
            SDL_Rect tryRect = { x, y, TILE_SIZE, TILE_SIZE };

            bool collision = false;

            for (const auto& obs : obstacles) {
                SDL_Rect rect = obs.getRect();
                if (SDL_HasIntersection(&tryRect, &rect)) {
                    collision = true;
                    break;
                }
            }

            for (const auto& e : enemies) {
                SDL_Rect rect = e.getRect();
                if (SDL_HasIntersection(&tryRect, &rect)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                enemies.emplace_back(enemyTexture, x, y, health);
                break;
            }

            attempts++;
        }
        spawned++;
    }
}


bool noEnemiesAlive(const std::vector<Enemy>& enemies) {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) return false;
    }
    return true;
}
