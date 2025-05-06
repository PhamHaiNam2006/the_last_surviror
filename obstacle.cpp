#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int w, int h, int t) {
    rect = {x,y,w,h};
    type=t;
    /*80=wall
    18=healing on/off

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

SDL_Rect Obstacle::getTileClip() const {
    int tilesPerRow = 256 / 16;
    int tileX = type % tilesPerRow;
    int tileY = type / tilesPerRow;
    return { tileX * 16, tileY * 16, 16, 16 };
}

