#include "enemy_spawner.h"
#include <cstdlib>
#include <random>

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
