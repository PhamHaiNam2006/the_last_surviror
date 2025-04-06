#include "character.h"
#include "movement.h"
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

MainChar::MainChar(int health) : health(health) {
    width = 64;
    height = 64;
    x = 800 / 2 - 32;
    y = 600 / 2 - 32;
    facingRight = true;

    showSlash = false;
    slashStartTime = 0;
    swordTexture = nullptr;
}

void MainChar::getpos(int &dx,int &dy){
    x=dx;y=dy;
}

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

void MainChar::triggerSlash() {
    showSlash = true;
    slashStartTime = SDL_GetTicks();
}

void MainChar::renderSlash(SDL_Renderer* renderer) {
    swordTexture = IMG_LoadTexture(renderer,"png_file/main_char/sword_slashing.png");
    if (!showSlash || !swordTexture) return;

    const int swordWidth = 64;
    const int swordHeight = 128;

    SDL_Rect srcRect = { 0, 0, swordWidth, swordHeight };
    SDL_Rect destRect;

    int verticalOffset = (swordHeight - height) / 2;

    if (facingRight) {
        destRect = {
            x + width,
            y - verticalOffset,
            swordWidth,
            swordHeight
        };
        SDL_RenderCopyEx(renderer, swordTexture, &srcRect, &destRect, 0, nullptr, SDL_FLIP_NONE);
    } else {
        destRect = {
            x - swordWidth,
            y - verticalOffset,
            swordWidth,
            swordHeight
        };
        SDL_RenderCopyEx(renderer, swordTexture, &srcRect, &destRect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    }

    if (SDL_GetTicks() - slashStartTime > 200) {
        showSlash = false;
    }
}
