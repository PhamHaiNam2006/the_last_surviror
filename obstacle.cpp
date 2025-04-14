#include "obstacle.h"
#include <SDL_image.h>

Obstacle::Obstacle(int x, int y, int w, int h, bool it) {
    rect = {x, y, w, h};
    obs=it;
}

void Obstacle::render(SDL_Renderer* renderer) const {
    SDL_Surface* surface = IMG_Load("png_file/environment/wall.png");

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!obs) {
        for (int i = rect.x; i < rect.x + rect.w; i += 32) {
            for (int j = rect.y; j < rect.y + rect.h; j += 32) {
                SDL_Rect tileRect = {i, j, 32, 32};
                SDL_RenderCopy(renderer, texture, nullptr, &tileRect);
            }
        }
        SDL_DestroyTexture(texture);
    } else {
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        SDL_RenderFillRect(renderer,&rect);
    }

}

void Obstacle::health_render(SDL_Renderer* renderer) const{

}

SDL_Rect Obstacle::getRect() const {
    return rect;
}

#include <SDL.h>
#include <SDL_image.h>


