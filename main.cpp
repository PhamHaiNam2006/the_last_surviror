#include <SDL.h>
#include <vector>
#include "movement.h"
#include "defs.h"
#include "obstacle.h"
#include "character.h"

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Arrow Key Movement", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture1 = IMG_LoadTexture(renderer,"png_file/main_char/main_right.png");
    SDL_Texture* texture2 = IMG_LoadTexture(renderer,"png_file/main_char/main_left.png");
    SDL_Texture* texture3 = IMG_LoadTexture(renderer,"png_file/main_char/main_up.png");
    SDL_Texture* texture4 = IMG_LoadTexture(renderer,"png_file/main_char/main_down.png");
    bool running = true;
    SDL_Event event;

    SDL_Rect rect = { SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50 };
    Movement movement(5);
    MainChar mainChar(100);

    vector<Obstacle> obstacles = {
        Obstacle(0, 0, 32, 600),
        Obstacle(0, 0, 800, 32),
        Obstacle(0, 568, 800, 32)
    };
    bool isLeft=false;
    bool isRight=true;
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
                        isRight = false;
                        break;
                    case SDLK_RIGHT:
                        isRight = true;
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

        for (const auto& obstacle : obstacles) {
            obstacle.render(renderer);
        }
        if(isRight){
            SDL_RenderCopy(renderer,texture1,NULL,&rect);
        } else if (isLeft) {
            SDL_RenderCopy(renderer,texture2,NULL,&rect);
        }
        if(isUp){
            SDL_RenderCopy(renderer,texture3,NULL,&rect);
        } else if(isDown) {
            SDL_RenderCopy(renderer,texture4,NULL,&rect);
        }

        mainChar.renderHealthBar(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Giới hạn tốc độ khung hình khoảng 60 FPS

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
