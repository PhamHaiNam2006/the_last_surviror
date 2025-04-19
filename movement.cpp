#include "movement.h"

bool willCollide(const SDL_Rect& nextPos, const std::vector<Obstacle>& obstacles) {
    for (const auto& o : obstacles) {
        SDL_Rect oRect = o.getRect();
        if (SDL_HasIntersection(&nextPos, &oRect)) {
            return true;
        }
    }
    return false;
}

void handleMovement(SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    SDL_Rect nextPos = playerRect;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_UP])    nextPos.y -= 16;
    if (keystates[SDL_SCANCODE_DOWN])  nextPos.y += 16;
    if (keystates[SDL_SCANCODE_LEFT])  nextPos.x -= 16;
    if (keystates[SDL_SCANCODE_RIGHT]) nextPos.x += 16;

    if (!willCollide(nextPos, obstacles)) {
        playerRect = nextPos;
    }
}
