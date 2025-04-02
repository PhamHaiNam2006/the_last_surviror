#include "character.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

void MainChar::renderHealthBar(SDL_Renderer* renderer) const {
    SDL_Rect healthBarBackground = { 20, 20, 100, 10 };
    SDL_Rect healthBar = { 20, 20, (100 * health) / 100, 10 };

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &healthBarBackground);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthBar);
}

MainChar::MainChar(int health) : health(health) {}
