#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <map>
#include <iostream>
#include "movement.h"
#include "defs.h"
#include "obstacle.h"
#include "character.h"

using namespace std;

void renderTiledBackground(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    int tileSize = 16;
    SDL_Texture* tileTexture = IMG_LoadTexture(renderer,"png_file/environment/background_rock.png");

    SDL_Rect srcRect = { 0, 0, tileSize, tileSize };
    SDL_Rect destRect;

    for (int y = 0; y < screenHeight; y += tileSize) {
        for (int x = 0; x < screenWidth; x += tileSize) {
            destRect = { x, y, tileSize, tileSize };
            SDL_RenderCopy(renderer, tileTexture, &srcRect, &destRect);
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Arrow Key Movement", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    SDL_Rect rect = { SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 64, 64 };
    Movement movement(25);
    MainChar player(100);

    map<int,vector<Obstacle>> obstacles = {
        {1, {Obstacle(0, 0, 32, 600),Obstacle(0, 0, 800, 32),Obstacle(0, 568, 800, 32)}},
        {2, {Obstacle(0, 0, 800, 32),Obstacle(0, 568, 800, 32)}},
        {3, {Obstacle(0, 0, 800, 32),Obstacle(0, 568, 800, 32),Obstacle(768, 0, 32, 600)}}
    };
    bool isLeft=false;
    bool isUp=false;
    bool isDown=false;
    player.facingRight = true;
    bool moving=true;
    int n=1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                movement.handleEvent(event);
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        isLeft = true;
                        player.facingRight = false;
                        break;
                    case SDLK_RIGHT:
                        isLeft = false;
                        player.facingRight = true;
                        break;
                    case SDLK_UP:
                        isUp = true;
                        isDown = false;
                        break;
                    case SDLK_DOWN:
                        isDown = true;
                        isUp = false;
                        break;
                }
                if (event.key.keysym.sym == SDLK_SPACE) {
                    player.triggerSlash();
                    moving=false;
                    SDL_Delay(200);
                }
            }
            if (event.type == SDL_KEYUP) {
                movement.handleEvent(event);
                switch (event.key.keysym.sym) {
                    case SDLK_UP: isUp = false; break;
                    case SDLK_DOWN: isDown = false; break;
                    case SDLK_SPACE: moving = true; break;
                }
            }
        }
        if(moving){
            movement.update(rect, obstacles[n], n);
        }
        player.getpos(rect.x,rect.y);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderTiledBackground(renderer, 800, 600);

        for (const auto& obstacle : obstacles[n]) {
            obstacle.render(renderer);
        }
        player.displayMain(renderer,rect,isLeft,isUp,isDown);

        player.renderSlash(renderer);

        player.renderHealthBar(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);


    }
    SDL_DestroyTexture(player.swordTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
