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
    ATTACKING,
    DEAD
};

void RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

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
    SDL_Texture* slashTexture = IMG_LoadTexture(renderer, "png_file/main_char/slash.png");

    vector<Enemy> enemy = {
        Enemy(enemyTexture, 160, 320),
        Enemy(enemyTexture, 200, 200)
    };

    SDL_Rect playerSrc = { 0, 0, 12, 16 };
    SDL_Rect playerDest = { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 12, 24, 32 };

    int playerHealth = 100;
    int maxHealth = 100;

    int playerStamina = 100;
    int maxStamina = 100;
    Uint32 runOutStamina = 0;
    const Uint32 runCooldown = 2000;

    bool playerAttacking = false;
    Uint32 attackStartTime = 0;
    Uint32 lastAttackTime = 0;
    const Uint32 attackCooldown = 500;
    SDL_Rect playerAttackBox = {0, 0, 0, 0};

    bool showSlash = false;
    SDL_Rect slashSrc = {0, 0, 12, 48};
    SDL_Rect slashDest = {0, 0, 24, 96};
    Uint32 slashStartTime = 0;
    const Uint32 slashDuration = 200;

    mapManager.loadMaps();
    mapManager.setCurrentMap(0);
    int n=0;

    TTF_Font* font = TTF_OpenFont("pixel_font.ttf", 24);
    GameState gameState = GameState::MENU;
    PlayerState playerState = PlayerState::IDLE;

    bool running = true;
    SDL_Event event;

    bool facingLeft = false;
    int currentFrame = 0;
    Uint32 lastAnimTime = 0;
    const Uint32 animDelay = 100;
    int sprint;

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
            sprint = 1;

            if (keystates[SDL_SCANCODE_UP] || keystates[SDL_SCANCODE_DOWN] || keystates[SDL_SCANCODE_LEFT] || keystates[SDL_SCANCODE_RIGHT]) {
                playerState = PlayerState::WALKING;
                if (keystates[SDL_SCANCODE_LEFT]) facingLeft = true;
                else if (keystates[SDL_SCANCODE_RIGHT]) facingLeft = false;
            } else {
                playerState = PlayerState::IDLE;
            }

            if (keystates[SDL_SCANCODE_LSHIFT] && playerStamina >= 2 && SDL_GetTicks() - runOutStamina >= runCooldown) {
                sprint = 2;
            } else if (playerStamina == 0) {
                runOutStamina = SDL_GetTicks();
            }

            if (playerState == PlayerState::WALKING && sprint == 2) {
                playerStamina = max(0, playerStamina - 2);
            } else if (playerStamina < maxStamina) {
                playerStamina += 1;
            }

            if (keystates[SDL_SCANCODE_SPACE] && SDL_GetTicks() - lastAttackTime >= attackCooldown && !playerAttacking) {
                playerAttacking = true;
                attackStartTime = SDL_GetTicks();
                lastAttackTime = SDL_GetTicks();

                if (facingLeft) {
                    playerAttackBox = { playerDest.x - 24, playerDest.y, 32, 32 };
                    slashDest = { playerDest.x - 24, playerDest.y - 16, 24, 64 };
                } else {
                    playerAttackBox = { playerDest.x + playerDest.w, playerDest.y, 32, 32 };
                    slashDest = { playerDest.x + playerDest.w, playerDest.y - 16, 24, 64 };
                }

                showSlash = true;
                slashStartTime = SDL_GetTicks();
                currentFrame = 13;
            } else if (SDL_GetTicks() - lastAttackTime >= slashDuration) {
                handleMovement(playerDest, mapManager.getCurrentMap(), n, sprint);
            }

            if (playerAttacking) {
                Uint32 elapsed = SDL_GetTicks() - attackStartTime;

                if (elapsed > 300) {
                    playerAttacking = false;
                    playerAttackBox = {0, 0, 0, 0};
                } else if (elapsed > 200) {
                    currentFrame = 15;
                } else if (elapsed > 100) {
                    currentFrame = 14;
                } else {
                    currentFrame = 13;
                }
            }

            if (showSlash && SDL_GetTicks() - slashStartTime > slashDuration) {
                showSlash = false;
            }

            if (playerState == PlayerState::WALKING && now - lastAnimTime > animDelay) {
                currentFrame = (currentFrame >= 7) ? 2 : currentFrame + 1;
                lastAnimTime = now;
            } else if (playerState == PlayerState::IDLE && now - lastAnimTime > 800) {
                currentFrame = (currentFrame == 0) ? 1 : 0;
                lastAnimTime = now;
            } else if (playerState == PlayerState::ATTACKING) {
                currentFrame = (currentFrame >= 15) ? 13 : currentFrame + 1;
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

            if (playerAttacking) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
                SDL_Rect renderBox = {
                    playerAttackBox.x,
                    playerAttackBox.y-16,
                    playerAttackBox.w-8,
                    playerAttackBox.h*2
                };
                SDL_RenderDrawRect(renderer, &renderBox);
            }

            SDL_RenderCopyEx(renderer, playerTexture, &playerSrc, &playerDest, 0, nullptr, flip);
            if (showSlash) {
                SDL_RenderCopyEx(renderer, slashTexture, &slashSrc, &slashDest, 0, nullptr, flip);
            }

            if (showSlash && enemy[n].isAlive()) {
                SDL_Rect enemyHitBox = enemy[n].getHitbox();
                if (SDL_HasIntersection(&slashDest, &enemyHitBox)) {
                    enemy[n].kill();
                }
            }

            if (enemy[n].isAlive()) {
                enemy[n].update(playerDest, mapManager.getCurrentMap());
                enemy[n].render(renderer);
            }

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect healthBack = { 20, 20, 200, 20 };
            SDL_Rect healthFill = { 20, 20, 200 * playerHealth / maxHealth, 20 };

            SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
            SDL_RenderFillRect(renderer, &healthBack);
            SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
            SDL_RenderFillRect(renderer, &healthFill);

            SDL_Rect staminaBack = { 20, 45, 200, 15 };
            SDL_Rect staminaFill = { 20, 45, 200 * playerStamina / maxStamina, 15 };

            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderFillRect(renderer, &staminaBack);
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &staminaFill);

            SDL_RenderPresent(renderer);

        } else if(gameState == GameState::MENU) {
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
