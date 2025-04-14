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

MainChar::MainChar(int health) : health(health), lastDamageTime(0), damageCooldown(1000){
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

void MainChar::displayMain(SDL_Renderer* renderer, SDL_Rect rect, bool isLeft, bool isUp, bool isDown) {
    const char* path = "png_file/main_char/main_right.png";
    if (isUp) {
        path = "png_file/main_char/main_up.png";
    } else if (isDown) {
        path = "png_file/main_char/main_down.png";
    } else if (isLeft) {
        path = "png_file/main_char/main_left.png";
    }
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }
}
void MainChar::triggerSlash(int moving) {
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

void MainChar::reduceHealth(int &amount) {
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastDamageTime >= damageCooldown && amount > 0) {
        health = (health - amount > 0) ? health - amount : 0;
        lastDamageTime = currentTime;
        amount = 0;
    } else {
        amount = 0;
    }
}
