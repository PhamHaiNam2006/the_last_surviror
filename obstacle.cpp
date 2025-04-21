#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int w, int h, int t) {
    rect = {x,y,w,h};
    type=t;
    /*80=wall
    65=baricage
    0=title*/
}

void Obstacle::render(SDL_Renderer* renderer, SDL_Texture* texture) const {
    SDL_Rect srcRect = {
        (type % 16) * 16,
        (type / 16) * 16,
        16,
        16
    };

    SDL_Rect destRect;
    for (int y = rect.y; y < rect.y + rect.h; y += 32) {
        for (int x = rect.x; x < rect.x + rect.w; x += 32) {
            destRect = { x, y, 32, 32 };
            SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
        }
    }
}

