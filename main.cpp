#include <SDL.h>
#include <vector>
#include "movement.h"
#include "defs.h"
#include "obstacle.h"

using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Arrow Key Movement", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    SDL_Rect rect = { SCREEN_WIDTH , SCREEN_HEIGHT / 2 -25, 50, 50 };
    Movement movement(10);

    vector<Obstacle> obstacles = {
        Obstacle(0, 100, 100, 100),
        Obstacle(100, 300, 50, 50),
        Obstacle(600, 450, 100, 100)
    };

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else {
                movement.handleEvent(event);
            }
        }

        movement.update(rect, obstacles);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (const auto& obstacle : obstacles) {
            obstacle.render(renderer);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Giới hạn tốc độ khung hình khoảng 60 FPS

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
