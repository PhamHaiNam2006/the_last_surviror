#include "movement.h"

Movement::Movement(int speed) : speed(speed), dx(), dy() {}

void Movement::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP: dy = -speed; break;
            case SDLK_DOWN: dy = speed; break;
            case SDLK_LEFT: dx = -speed; break;
            case SDLK_RIGHT: dx = speed; break;
        }
    } else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_UP: case SDLK_DOWN: dy = 0; break;
            case SDLK_LEFT: case SDLK_RIGHT: dx = 0; break;
        }
    }
}

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    return (a.x < b.x + b.w && a.x + a.w > b.x &&
            a.y < b.y + b.h && a.y + a.h > b.y);
}

void Movement::update(SDL_Rect& rect, const std::vector<Obstacle>& obstacles, int &n) {
    rect.x += dx;
    rect.y += dy;

    // Wrap-around effect when touching screen boundaries
    if (rect.x < 0) rect.x = 800 - rect.w, n--;
    if (rect.y < 0) rect.y = 600 - rect.h;
    if (rect.x + rect.w > 800) rect.x = 0, n++;
    if (rect.y + rect.h > 600) rect.y = 0;

    // Collision with obstacles
    for (const auto& obstacle : obstacles) {
        if (checkCollision(rect, obstacle.getRect())) {
            rect.x -= dx;
            rect.y -= dy;
            break;
        }
    }
}

