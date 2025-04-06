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

void MainChar::displayMain(SDL_Renderer *renderer, SDL_Rect rect, bool isLeft, bool isUp, bool isDown) {
    SDL_Texture* texture1 = IMG_LoadTexture(renderer,"png_file/main_char/main_right.png");
    SDL_Texture* texture2 = IMG_LoadTexture(renderer,"png_file/main_char/main_left.png");
    SDL_Texture* texture3 = IMG_LoadTexture(renderer,"png_file/main_char/main_up.png");
    SDL_Texture* texture4 = IMG_LoadTexture(renderer,"png_file/main_char/main_down.png");
    if(isLeft){
        SDL_RenderCopy(renderer,texture2,NULL,&rect);
    } else {
        SDL_RenderCopy(renderer,texture1,NULL,&rect);
    }
    if(isUp){
        SDL_RenderCopy(renderer,texture3,NULL,&rect);
    } else if(isDown) {
        SDL_RenderCopy(renderer,texture4,NULL,&rect);
    }
}
