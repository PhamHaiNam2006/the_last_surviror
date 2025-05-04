#include "movement.h"
#include "defs.h"

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

void handleMovement(SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles, int sprint) {
    SDL_Rect simulatedHitbox;
    const SDL_Rect hitboxOffset = { -8, -4, 32, 32 };
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    int speed = 4 * sprint;

    SDL_Rect originalPos = playerRect;

    if (keystates[SDL_SCANCODE_UP]) {
        playerRect.y -= speed;
        simulatedHitbox = {
            playerRect.x + hitboxOffset.x,
            playerRect.y + hitboxOffset.y,
            hitboxOffset.w,
            hitboxOffset.h
        };
        if (willCollide(simulatedHitbox, obstacles)) {
            playerRect.y = originalPos.y;
        }
    }
    if (keystates[SDL_SCANCODE_DOWN]) {
        playerRect.y += speed;
        simulatedHitbox = {
            playerRect.x + hitboxOffset.x,
            playerRect.y + hitboxOffset.y,
            hitboxOffset.w,
            hitboxOffset.h
        };
        if (willCollide(simulatedHitbox, obstacles)) {
            playerRect.y = originalPos.y;
        }
    }
    if (keystates[SDL_SCANCODE_LEFT]) {
        playerRect.x -= speed;
        simulatedHitbox = {
            playerRect.x + hitboxOffset.x,
            playerRect.y + hitboxOffset.y,
            hitboxOffset.w,
            hitboxOffset.h
        };
        if (willCollide(simulatedHitbox, obstacles)) {
            playerRect.x = originalPos.x;
        }
    }
    if (keystates[SDL_SCANCODE_RIGHT]) {
        playerRect.x += speed;
        simulatedHitbox = {
            playerRect.x + hitboxOffset.x,
            playerRect.y + hitboxOffset.y,
            hitboxOffset.w,
            hitboxOffset.h
        };
        if (willCollide(simulatedHitbox, obstacles)) {
            playerRect.x = originalPos.x;
        }
    }
    if (playerRect.x < 0) playerRect.x = 0;
    if (playerRect.y < 0) playerRect.y = 0;
    if (playerRect.x + playerRect.w > SCREEN_WIDTH) playerRect.x = SCREEN_WIDTH - playerRect.w;
    if (playerRect.y + playerRect.h > SCREEN_HEIGHT) playerRect.y = SCREEN_HEIGHT - playerRect.h;
}


