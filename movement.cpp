#include "movement.h"

bool willCollide(const SDL_Rect& nextPos, const std::vector<Obstacle>& obstacles) {
    for (const auto& o : obstacles) {
        int type = o.getType();
        SDL_Rect oRect = o.getRect();
        if (SDL_HasIntersection(&nextPos, &oRect)&&type!=0) {
            return true;
        }
    }
    return false;
}

void handleMovement(SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles) {
    SDL_Rect nextPlayerPos = playerRect;
    SDL_Rect simulatedHitbox;

    const SDL_Rect hitboxOffset = { -8, -4, 32, 32 };
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_UP]) {
        nextPlayerPos.y -= 4;
    } else if (keystates[SDL_SCANCODE_DOWN]) {
        nextPlayerPos.y += 4;
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        nextPlayerPos.x -= 4;
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        nextPlayerPos.x += 4;
    }

    simulatedHitbox = {
        nextPlayerPos.x + hitboxOffset.x,
        nextPlayerPos.y + hitboxOffset.y,
        hitboxOffset.w,
        hitboxOffset.h
    };

    if (!willCollide(simulatedHitbox, obstacles)) {
        playerRect = nextPlayerPos;
    }
}
