#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <vector>
#include <SDL.h>
#include "enemy.h"
#include "obstacle.h"

void spawnEnemies(std::vector<Enemy>& enemies, SDL_Texture* enemyTexture, int count, int mapWidth, int mapHeight, const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles);
bool noEnemiesAlive(const std::vector<Enemy>& enemies);

#endif
