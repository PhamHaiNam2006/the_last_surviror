#include "obstacle.h"
#include <SDL_image.h>

Obstacle::Obstacle(int x, int y, int w, int h) {
    rect = {x, y, w, h};
}

void Obstacle::render(SDL_Renderer* renderer) const {
    SDL_Surface* surface = IMG_Load("png_file/environment/wall.png");
    if (!surface) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green color fallback
        SDL_RenderFillRect(renderer, &rect);
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (texture) {
        for (int i = rect.x; i < rect.x + rect.w; i += 32) { // Assuming wall.png is 32x32
            for (int j = rect.y; j < rect.y + rect.h; j += 32) {
                SDL_Rect tileRect = {i, j, 32, 32};
                SDL_RenderCopy(renderer, texture, nullptr, &tileRect);
            }
        }
        SDL_DestroyTexture(texture);
    }
}

SDL_Rect Obstacle::getRect() const {
    return rect;
}
