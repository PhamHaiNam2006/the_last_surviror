#include "enemy_spawner.h"
#include <cstdlib>

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

void spawnEnemies(std::vector<Enemy>& enemies, SDL_Texture* enemyTexture, int count, int mapWidth, int mapHeight, const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    enemies.clear();
    const int TILE_SIZE = 32;
    const int MIN_DISTANCE = TILE_SIZE * 5;
    const int MAX_ATTEMPTS = 100;

    int spawned = 0;
    while (spawned < count) {
        int attempts = 0;
        while (attempts < MAX_ATTEMPTS) {
            int x = (rand() % ((mapWidth - TILE_SIZE) / TILE_SIZE)) * TILE_SIZE;
            int y = (rand() % ((mapHeight - TILE_SIZE) / TILE_SIZE)) * TILE_SIZE;
            SDL_Rect tryRect = { x, y, TILE_SIZE, TILE_SIZE };

            int dx = tryRect.x - playerRect.x;
            int dy = tryRect.y - playerRect.y;
            int distSquared = dx * dx + dy * dy;

            if (distSquared >= MIN_DISTANCE * MIN_DISTANCE && !collidesWithObstacles(tryRect, obstacles)) {
                enemies.emplace_back(enemyTexture, x, y, 100);
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
