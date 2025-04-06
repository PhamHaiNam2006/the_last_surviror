#include <SDL.h>
#include <vector>
#include "movement.h"
#include "defs.h"
#include "obstacle.h"
#include "character.h"

using namespace std;

void renderTiledBackground(SDL_Renderer* renderer, int screenWidth, int screenHeight) {
    int tileSize = 16; // Your tile is 16x16
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

    SDL_Rect rect = { SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50 };
    Movement movement(10);
    MainChar mainChar(100);

    vector<Obstacle> obstacles = {
        Obstacle(0, 0, 32, 600),
        Obstacle(0, 0, 800, 32),
        Obstacle(0, 568, 800, 32)
    };
    bool isLeft=false;
    bool isUp=false;
    bool isDown=false;
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
                        break;
                    case SDLK_RIGHT:
                        isLeft = false;
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
            }
            if (event.type == SDL_KEYUP) {
                movement.handleEvent(event);
                switch (event.key.keysym.sym) {
                    case SDLK_UP: isUp = false; break;
                    case SDLK_DOWN: isDown = false; break;
                }
            }
        }
        movement.update(rect, obstacles);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderTiledBackground(renderer, 800, 600);

        for (const auto& obstacle : obstacles) {
            obstacle.render(renderer);
        }
        mainChar.displayMain(renderer,rect,isLeft,isUp,isDown);

        mainChar.renderHealthBar(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
