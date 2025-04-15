#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <map>
#include <iostream>
#include "movement.h"
#include "defs.h"
#include "obstacle.h"
#include "character.h"

using namespace std;

enum class GameState {
    MENU,
    PLAYING
};

void renderStartScreen(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Start Game", white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int btnW = 200, btnH = 60;
    SDL_Rect buttonRect = {
        (SCREEN_WIDTH - btnW) / 2,
        (SCREEN_HEIGHT - btnH) / 2,
        btnW,
        btnH
    };

    SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255);
    SDL_RenderFillRect(renderer, &buttonRect);
    SDL_RenderCopy(renderer, textTexture, nullptr, &buttonRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    SDL_RenderPresent(renderer);
}


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
        {1, {Obstacle(0, 0, 32, 600,0),Obstacle(0, 0, 800, 32,0),Obstacle(0, 568, 800, 32,0),Obstacle(50,50,50,50,1)}},
        {2, {Obstacle(0, 0, 800, 32,0),Obstacle(0, 568, 800, 32,0)}},
        {3, {Obstacle(0, 0, 800, 32,0),Obstacle(0, 568, 800, 32,0),Obstacle(768, 0, 32, 600,0),Obstacle(400,300,50,50,1)}}
    };
    bool isLeft=false;
    bool isUp=false;
    bool isDown=false;
    player.facingRight = true;
    bool moving=true;
    int n=1;
    int amount=0;
    GameState gameState = GameState::MENU;
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("pixel_font.ttf", 24);

    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music* back_ground_music = Mix_LoadMUS("mp3_and_ogg_file/back_ground.ogg");
    Mix_Chunk* sword_slashing = Mix_LoadWAV("mp3_and_ogg_file/sword_attack.mp3");
    //Mix_PlayMusic(back_ground_music,-1);
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (gameState == GameState::MENU && event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                SDL_Rect startButton = {(SCREEN_WIDTH - 200) / 2, (SCREEN_HEIGHT - 60) / 2, 200, 60};
                if (x >= startButton.x && x <= startButton.x + startButton.w &&
                    y >= startButton.y && y <= startButton.y + startButton.h) {
                    gameState = GameState::PLAYING;
                }
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
                    player.triggerSlash(moving);
                    moving=false;
                    //Mix_PlayChannel(-1,sword_slashing,0);
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
            if (gameState == GameState::MENU) {
                renderStartScreen(renderer, font);
                continue;
            } else {
                if(moving){
            movement.update(rect, obstacles[n], n, amount);
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

        if (player.isSlashing()) {
            SDL_Rect slashHitbox = player.getSlashHitbox();
            for (auto& obs : obstacles[n]) {
                if (!obs.isDestroyed() && obs.redcube() && checkCollision(slashHitbox, obs.getRect())) {
                    obs.destroy();
                }
            }
        }

        player.renderHealthBar(renderer);

        player.reduceHealth(amount);

        player.noHealth(running);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
            }
        }



    }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyTexture(player.swordTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
