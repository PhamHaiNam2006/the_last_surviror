#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include "defs.h"
#include "obstacle.h"
#include "movement.h"
#include "map_manager.h"
#include "enemy.h"

using namespace std;

const char* WINDOW_TITLE = "Dungeon Explorer";
MapManager mapManager;

enum class GameState {
    MENU,
    PLAYING
};

enum class PlayerState {
    IDLE,
    WALKING,
    DEAD
};

void renderStartScreen(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Start Game", white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect buttonRect = {
        (SCREEN_WIDTH - 200) / 2,
        (SCREEN_HEIGHT - 60) / 2,
        200,
        60
    };

    SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255);
    SDL_RenderFillRect(renderer, &buttonRect);
    SDL_RenderCopy(renderer, textTexture, nullptr, &buttonRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "png_file/main_char/warrior.png");
    SDL_Texture* tileTexture = IMG_LoadTexture(renderer, "png_file/environment/tiles_sewers.png");
    SDL_Texture* enemyTexture = IMG_LoadTexture(renderer, "png_file/enemy/rat.png");
    Enemy enemy(enemyTexture, 160, 320);

    SDL_Rect playerSrc = { 0, 0, 12, 16 };
    SDL_Rect playerDest = { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 12, 24, 32 };
    SDL_Rect playerHitbox = { playerDest.x, playerDest.y, 24, 32 };
    mapManager.loadMaps();
    mapManager.setCurrentMap(0);
    int n=0;
    int m=0;

    TTF_Font* font = TTF_OpenFont("pixel_font.ttf", 24);
    GameState gameState = GameState::MENU;
    PlayerState playerState = PlayerState::IDLE;

    bool running = true;
    SDL_Event event;


    bool facingLeft = false;
    int currentFrame = 0;
    Uint32 lastAnimTime = 0;
    const Uint32 animDelay = 100;

    while (running) {
        Uint32 now = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (gameState == GameState::MENU && event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                SDL_Point mousePoint = {x, y};
                SDL_Rect startButton = { (SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 60) / 2, 200, 60 };
                if (SDL_PointInRect(&mousePoint, &startButton)) {
                    gameState = GameState::PLAYING;
                }
            }
        }

        if (gameState == GameState::PLAYING) {
            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            handleMovement(playerDest, mapManager.getCurrentMap(),n);
            handleMovement(playerHitbox, mapManager.getCurrentMap(),m);

            if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_RIGHT]) {
                playerState = PlayerState::WALKING;
                if (keystates[SDL_SCANCODE_LEFT]) facingLeft = true;
                else if (keystates[SDL_SCANCODE_RIGHT]) facingLeft = false;
            } else {
                playerState = PlayerState::IDLE;
            }

            if (playerState == PlayerState::WALKING && now - lastAnimTime > animDelay) {
                currentFrame = (currentFrame >= 7) ? 2 : currentFrame + 1;
                lastAnimTime = now;
            } else if (playerState == PlayerState::IDLE && now - lastAnimTime > 800) {
                currentFrame = (currentFrame == 0) ? 1 : 0;
                lastAnimTime = now;
            }


            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            mapManager.setCurrentMap(n);
            for (const auto& o : mapManager.getCurrentMap()) {
                SDL_Rect dest = o.getRect();
                SDL_Rect src = o.getTileClip();
                SDL_RenderCopy(renderer, tileTexture, &src, &dest);
            }

            playerSrc.x = currentFrame * 12;
            SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

            SDL_RenderCopyEx(renderer, playerTexture, &playerSrc, &playerDest, 0, nullptr, flip);
            enemy.update(playerHitbox, mapManager.getCurrentMap());
            enemy.render(renderer);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &playerHitbox);
            SDL_RenderPresent(renderer);


        } else {
            renderStartScreen(renderer, font);
        }
    SDL_Delay(16);
    }


    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
