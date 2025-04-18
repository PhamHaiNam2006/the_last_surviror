#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <map>
#include <iostream>
#include <chrono>
#include <cmath>
#include "defs.h"

using namespace std;

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
    int tileSize = 32;
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
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "png_file/main_char/warrior.png");
    SDL_Rect playerSrc = { 0, 0, 12, 16 };
    SDL_Rect playerDest = { SCREEN_WIDTH / 2 - 16, SCREEN_HEIGHT / 2 - 16, 24, 32 };

    bool running = true;
    SDL_Event event;

    GameState gameState = GameState::MENU;
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("pixel_font.ttf", 24);

    PlayerState playerState = PlayerState::IDLE;
    bool facingLeft = false;
    int currentFrame = 0;
    Uint32 lastAnimTime = 0;
    const Uint32 animDelay = 100;

    SDL_Point playerPos = { playerDest.x, playerDest.y };
    SDL_Point targetPos = playerPos;
    bool isMoving = false;
    const float moveSpeed = 0.2f;

    Uint32 lastMoveTime = 0;
    const Uint32 moveCooldown = 100;

    SDL_Rect playerHitbox = { playerDest.x-4, playerDest.y, 32, 32 };

    while (running) {
        Uint32 currentTime = SDL_GetTicks();

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
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);

        if (gameState == GameState::PLAYING && !isMoving && currentTime - lastMoveTime >= moveCooldown) {
            bool moved = false;
            if (keystates[SDL_SCANCODE_UP]) {
                targetPos.y -= 32;
                moved = true;
            } else if (keystates[SDL_SCANCODE_DOWN]) {
                targetPos.y += 32;
                moved = true;
            } else if (keystates[SDL_SCANCODE_LEFT]) {
                targetPos.x -= 32;
                facingLeft = true;
                moved = true;
            } else if (keystates[SDL_SCANCODE_RIGHT]) {
                targetPos.x += 32;
                facingLeft = false;
                moved = true;
            }
            if (moved) {
                isMoving = true;
                lastMoveTime = currentTime;
                playerState = PlayerState::WALKING;
            } else {
                playerState = PlayerState::IDLE;
            }
        }

        if (isMoving) {
            int dx = targetPos.x - playerPos.x;
            int dy = targetPos.y - playerPos.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance > 0) {
                float step = moveSpeed * (SDL_GetTicks() - lastMoveTime);
                if (distance <= step) {
                    playerPos = targetPos;
                    isMoving = false;
                    playerState = PlayerState::IDLE;
                } else {
                    playerPos.x += static_cast<int>((dx / distance) * step);
                    playerPos.y += static_cast<int>((dy / distance) * step);
                }
                playerDest.x = playerPos.x;
                playerDest.y = playerPos.y;
                playerHitbox.x = playerPos.x-4;
                playerHitbox.y = playerPos.y;
            }
        }

        Uint32 now = SDL_GetTicks();
        if (playerState == PlayerState::WALKING && now - lastAnimTime > animDelay) {
            currentFrame++;
            if (currentFrame > 7) currentFrame = 2;
            lastAnimTime = now;
        } else if (playerState == PlayerState::IDLE) {
            if (now - lastAnimTime > 400) {
                currentFrame = (currentFrame == 0) ? 1 : 0;
                lastAnimTime = now;
            }
        }

        if (gameState == GameState::MENU) {
            renderStartScreen(renderer, font);
        } else if (gameState == GameState::PLAYING) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            renderTiledBackground(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            playerSrc.x = currentFrame * 12;
            playerSrc.y = 0;
            playerSrc.w = 12;
            playerSrc.h = 16;

            SDL_RendererFlip flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            SDL_RenderCopyEx(renderer, playerTexture, &playerSrc, &playerDest, 0, nullptr, flip);

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &playerHitbox);

            SDL_RenderPresent(renderer);
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
