#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int w, int h) {
    rect = {x, y, w, h};
}

void Obstacle::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Obstacle::getRect() const {
    return rect;
}
