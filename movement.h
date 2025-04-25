#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <SDL.h>
#include <vector>
#include "obstacle.h"

bool willCollide(const SDL_Rect& nextPos, const std::vector<Obstacle>& obstacles);
void handleMovement(SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles);


#endif
