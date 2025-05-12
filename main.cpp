#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include "defs.h"
#include "obstacle.h"
#include "movement.h"
#include "map_data.h"
#include "enemy.h"
#include "enemy_spawner.h"
#include "music_player.h"

using namespace std;

const char* WINDOW_TITLE = "Dungeon Explorer";

enum class GameState {
    MENU,
    TUTORIAL,
    OPTIONS,
    QUIT,
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

void renderOption(SDL_Renderer* renderer, TTF_Font* font, GameState gameState, int musicVol, int sfxVol) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 180);
    SDL_Rect popup = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    SDL_RenderFillRect(renderer, &popup);

    SDL_Color white = {255, 255, 255};

    RenderText(renderer, font, "Music Volume", popup.x + 50, popup.y + 30, white);
    RenderText(renderer, font, "SFX Volume", popup.x + 50, popup.y + 120, white);

    const int buttonW = 50, buttonH = 30;
    int values[] = {0, 25, 50, 75, 100};

    SDL_Rect musicButtons[5];
    SDL_Rect sfxButtons[5];

    for (int i = 0; i < 5; ++i) {
        musicButtons[i] = { popup.x + 50 + i * (buttonW + 10), popup.y + 60, buttonW, buttonH };
        SDL_SetRenderDrawColor(renderer, (values[i] == musicVol ? 0 : 120), 180, 120, 255);
        SDL_RenderFillRect(renderer, &musicButtons[i]);
        RenderText(renderer, font, std::to_string(values[i]), musicButtons[i].x + 15, musicButtons[i].y + 5, white);

        sfxButtons[i] = { popup.x + 50 + i * (buttonW + 10), popup.y + 150, buttonW, buttonH };
        SDL_SetRenderDrawColor(renderer, (values[i] == sfxVol ? 0 : 120), 120, 180, 255);
        SDL_RenderFillRect(renderer, &sfxButtons[i]);
        RenderText(renderer, font, std::to_string(values[i]), sfxButtons[i].x + 15, sfxButtons[i].y + 5, white);
    }

    SDL_Rect backButton = { popup.x + 50, popup.y + popup.h - 80, 120, 40 };
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &backButton);
    RenderText(renderer, font, "Back", backButton.x + 20, backButton.y + 10, white);

    if (gameState == GameState::PLAYING) {
        SDL_Rect toMenu = { popup.x + popup.w - 170, popup.y + popup.h - 80, 120, 40 };
        SDL_SetRenderDrawColor(renderer, 150, 50, 50, 255);
        SDL_RenderFillRect(renderer, &toMenu);
        RenderText(renderer, font, "To Menu", toMenu.x + 10, toMenu.y + 10, white);
    }

    SDL_RenderPresent(renderer);
}


void renderTutorialScreen(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};

    const char* lines[] = {
        "Press Arrow keys to move",
        "Press Space to attack",
        "You are fighting the enemy for a while, so no natural healing or healing potion",
        "Enemies will come in large wave",
        "Your objective in this game is to survive for 5 minutes as you see a big monster comming in your way",
        "The game ends after 5 minutes or if the player dies."
    };

    int lineCount = sizeof(lines) / sizeof(lines[0]);
    int spacing = 10;
    int y = 100;

    for (int i = 0; i < lineCount; ++i) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, lines[i], white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int textW, textH;
        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
        SDL_Rect dest = {
            (SCREEN_WIDTH - textW) / 2,
            y,
            textW,
            textH
        };
        SDL_RenderCopy(renderer, texture, nullptr, &dest);

        y += textH + spacing;

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_Rect backButton = {
        (SCREEN_WIDTH - 150) / 2,
        SCREEN_HEIGHT - 100,
        150,
        50
    };

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &backButton);

    SDL_Surface* backSurface = TTF_RenderText_Solid(font, "Back", white);
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);

    int textW, textH;
    SDL_QueryTexture(backTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect textRect = {
        backButton.x + (backButton.w - textW) / 2,
        backButton.y + (backButton.h - textH) / 2,
        textW,
        textH
    };
    SDL_RenderCopy(renderer, backTexture, nullptr, &textRect);

    SDL_FreeSurface(backSurface);
    SDL_DestroyTexture(backTexture);

    SDL_RenderPresent(renderer);
}

void renderStartScreen(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* titleFont) {
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255, 255};

    SDL_Surface* titleSurface1 = TTF_RenderText_Solid(titleFont, "The Last", white);
    SDL_Texture* titleTexture1 = SDL_CreateTextureFromSurface(renderer, titleSurface1);
    SDL_Rect titleRect1 = {(SCREEN_WIDTH - titleSurface1->w) / 2, 50, titleSurface1->w, titleSurface1->h};

    SDL_Surface* titleSurface2 = TTF_RenderText_Solid(titleFont, "Survivor", white);
    SDL_Texture* titleTexture2 = SDL_CreateTextureFromSurface(renderer, titleSurface2);
    SDL_Rect titleRect2 = {(SCREEN_WIDTH - titleSurface2->w) / 2, titleRect1.y + titleRect1.h + 10, titleSurface2->w, titleSurface2->h};

    SDL_RenderCopy(renderer, titleTexture1, nullptr, &titleRect1);
    SDL_RenderCopy(renderer, titleTexture2, nullptr, &titleRect2);

    SDL_FreeSurface(titleSurface1);
    SDL_FreeSurface(titleSurface2);
    SDL_DestroyTexture(titleTexture1);
    SDL_DestroyTexture(titleTexture2);

    const char* buttonLabels[4] = {"Start Game", "Tutorial", "Options", "Quit"};

    int buttonWidth = 200;
    int buttonHeight = 60;
    int spacingX = 40;
    int spacingY = 20;

    int totalRowWidth = buttonWidth * 2 + spacingX;
    int startX = (SCREEN_WIDTH - totalRowWidth) / 2;
    int startY = titleRect2.y + titleRect2.h + 60;

    for (int i = 0; i < 4; ++i) {
        int row = i / 2;
        int col = i % 2;

        SDL_Surface* textSurface = TTF_RenderText_Solid(font, buttonLabels[i], white);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect buttonRect = {
            startX + col * (buttonWidth + spacingX),
            startY + row * (buttonHeight + spacingY),
            buttonWidth,
            buttonHeight
        };

        SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255);
        SDL_RenderFillRect(renderer, &buttonRect);
        SDL_RenderCopy(renderer, textTexture, nullptr, &buttonRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    SDL_RenderPresent(renderer);
}


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "png_file/main_char/warrior.png");
    SDL_Texture* tileTexture = IMG_LoadTexture(renderer, "png_file/environment/tiles_sewers.png");
    SDL_Texture* enemyTexture = IMG_LoadTexture(renderer, "png_file/enemy/rat.png");
    SDL_Texture* slashTexture = IMG_LoadTexture(renderer, "png_file/main_char/slash.png");
    SDL_Texture* cogTexture = IMG_LoadTexture(renderer, "png_file/environment/option.png");

    vector<Enemy> enemies;

    MusicPlayer musicPlayer;

    musicPlayer.loadMusicFiles({
        ("ogg_file/city_1.ogg"),
        ("ogg_file/city_2.ogg"),
        ("ogg_file/city_3.ogg")
    });

    musicPlayer.loadSingleMusic("ogg_file/back_ground.ogg");

    GameState lastMusicState = GameState::PLAYING;

    musicPlayer.playSingleMusic();

    SDL_Rect playerSrc = { 0, 0, 12, 16 };
    SDL_Rect playerDest = { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 11, 24, 32 };

    int playerHealth = 100;
    int maxHealth = 100;

    int playerStamina = 100;
    int maxStamina = 100;
    Uint32 runOutStamina = 0;
    const Uint32 runCooldown = 250;

    bool playerAttacking = false;
    Uint32 attackStartTime = 0;
    Uint32 lastAttackTime = 0;
    const Uint32 attackCooldown = 500;
    SDL_Rect playerAttackBox = {0, 0, 0, 0};
    const Uint32 hitInvis = 400;
    const Uint32 playerInvis = 50;
    Uint32 lastHit = 0;
    Uint32 playerHit = 0;

    bool showSlash = false;
    SDL_Rect slashSrc = {0, 0, 12, 48};
    SDL_Rect slashDest = {0, 0, 24, 96};
    Uint32 slashStartTime = 0;
    const Uint32 slashDuration = 200;

    SDL_Rect cogButton = { SCREEN_WIDTH - 60, 20, 32, 32 };

    vector<Obstacle> obstacles = loadMapObstacles1();

    int mapWidth = SCREEN_WIDTH;
    int mapHeight = SCREEN_HEIGHT;
    int wave = 1;
    int n=5;

    spawnEnemies(enemies, enemyTexture, n, SCREEN_WIDTH, SCREEN_HEIGHT, playerDest, obstacles, 100);

    for (auto& e : enemies) {
        e.setAllEnemies(&enemies);
    }

    TTF_Font* font = TTF_OpenFont("pixel_font.ttf", 24);
    TTF_Font* titleFont = TTF_OpenFont("pixel_font.ttf", 96);
    GameState gameState = GameState::MENU;
    GameState previousState = GameState::MENU;

    PlayerState playerState = PlayerState::IDLE;

    bool running = true;
    SDL_Event event;

    bool facingLeft = false;
    int currentFrame = 0;
    Uint32 lastAnimTime = 0;
    const Uint32 animDelay = 100;
    int sprint;

    int musicVolume = 50;
    int sfxVolume = 50;

    while (running) {
        Uint32 now = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (gameState == GameState::MENU && event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;

                SDL_Point mousePoint = {x, y};

                int buttonWidth = 200;
                int buttonHeight = 60;
                int spacingX = 40;
                int spacingY = 20;

                int totalRowWidth = buttonWidth * 2 + spacingX;
                int startX = (SCREEN_WIDTH - totalRowWidth) / 2;

                int titleFontSize = 72;
                int titleHeight = titleFontSize * 2 + 10;
                int startY = 50 + titleHeight + 140;

                SDL_Rect buttons[4];

                for (int i = 0; i < 4; ++i) {
                    int row = i / 2;
                    int col = i % 2;
                    buttons[i] = {
                        startX + col * (buttonWidth + spacingX),
                        startY + row * (buttonHeight + spacingY),
                        buttonWidth,
                        buttonHeight
                    };

                    if (SDL_PointInRect(&mousePoint, &buttons[i])) {
                        switch (i) {
                            case 0: gameState = GameState::PLAYING; break;
                            case 1: previousState = gameState; gameState = GameState::TUTORIAL; break;
                            case 2: previousState = gameState; gameState = GameState::OPTIONS; break;
                            case 3: gameState = GameState::QUIT; break;
                        }
                    }
                }
            }
            if (gameState == GameState::TUTORIAL && event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x, y = event.button.y;
                SDL_Point mousePoint = {x, y};

                SDL_Rect backButton = {
                    (SCREEN_WIDTH - 150) / 2,
                    SCREEN_HEIGHT - 100,
                    150,
                    50
                };

                if (SDL_PointInRect(&mousePoint, &backButton)) {
                    gameState = previousState;
                }
            }
            if (gameState == GameState::OPTIONS && event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                SDL_Rect backButton = { SCREEN_WIDTH / 4 + 50, SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 2 - 80, 120, 40 };
                if (mx >= backButton.x && mx <= backButton.x + backButton.w &&
                    my >= backButton.y && my <= backButton.y + backButton.h) {
                    gameState = previousState;
                }

                SDL_Rect toMenu = { SCREEN_WIDTH * 3/4 - 150, SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 2 - 80, 120, 40 };
                if (previousState == GameState::PLAYING && mx >= toMenu.x && mx <= toMenu.x + toMenu.w &&
                    my >= toMenu.y && my <= toMenu.y + toMenu.h) {
                    gameState = GameState::MENU;
                }

                int values[] = {0, 25, 50, 75, 100};
                for (int i = 0; i < 5; ++i) {
                    SDL_Rect musicBtn = { SCREEN_WIDTH / 4 + 50 + i * 60, SCREEN_HEIGHT / 4 + 60, 50, 30 };
                    SDL_Rect sfxBtn   = { SCREEN_WIDTH / 4 + 50 + i * 60, SCREEN_HEIGHT / 4 + 150, 50, 30 };

                    if (mx >= musicBtn.x && mx <= musicBtn.x + musicBtn.w && my >= musicBtn.y && my <= musicBtn.y + musicBtn.h) {
                        musicVolume = values[i];
                        Mix_VolumeMusic((MIX_MAX_VOLUME * musicVolume) / 100);
                    }

                    if (mx >= sfxBtn.x && mx <= sfxBtn.x + sfxBtn.w && my >= sfxBtn.y && my <= sfxBtn.y + sfxBtn.h) {
                        sfxVolume = values[i];
                    }
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && gameState == GameState::PLAYING) {
                previousState = gameState;
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Rect cogButton = { SCREEN_WIDTH - 60, 20, 32, 32 };

                SDL_Point p = {mouseX, mouseY};
                if (SDL_PointInRect(&p, &cogButton)) {
                    gameState = GameState::OPTIONS;
                }
            }
        }

        if (lastMusicState == GameState::MENU && gameState == GameState::PLAYING) {
            musicPlayer.startPlaylist();
            lastMusicState = gameState;
        } else if (lastMusicState == GameState::PLAYING && gameState == GameState::MENU) {
            musicPlayer.playSingleMusic();
            lastMusicState = gameState;
        }

        musicPlayer.update();

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
                sprint = 3;
            } else if (playerStamina == 0) {
                runOutStamina = SDL_GetTicks();
            }

            if (playerState == PlayerState::WALKING && sprint == 3) {
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
                handleMovement(playerDest, obstacles,sprint);
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

            for (const auto& o : obstacles) {
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

            if (showSlash) {
                Uint32 hit = SDL_GetTicks();
                for (auto& e : enemies) {
                    SDL_Rect r = e.getHitbox();
                    if (e.isAlive() && SDL_HasIntersection(&slashDest, &r) && hit - lastHit >= hitInvis) {
                        e.getDamage(50);
                        lastHit = hit;
                        break;
                    }
                }
            }

            for (auto& e : enemies) {
                if (e.isAlive()) {
                    e.update(playerDest, obstacles);
                    e.render(renderer);
                    Uint32 hit = SDL_GetTicks();
                    if (e.touchingPlayer(playerDest)&&hit - playerHit >= playerInvis){
                        playerHealth-=1;
                        playerHit = hit;
                    }
                }
            }
            if (playerHealth == 0) {
                running = false;
            }

            enemies.erase(
                remove_if(enemies.begin(), enemies.end(),
                                [](const Enemy& e) { return !e.isAlive(); }),
                enemies.end());

            if (noEnemiesAlive(enemies)) {
                n++;
                wave++;
                spawnEnemies(enemies, enemyTexture, n, mapWidth, mapHeight, playerDest, obstacles, 100 * (1+(float)(wave-1)*0.05));
                for (auto& e : enemies) {
                    e.setAllEnemies(&enemies);
                }
            }

            SDL_RenderCopy(renderer, cogTexture, nullptr, &cogButton);

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
            renderStartScreen(renderer, font, titleFont);
        } else if(gameState == GameState::QUIT) {
            running = false;
        } else if (gameState == GameState::TUTORIAL) {
            renderTutorialScreen(renderer, font);
        } else if (gameState == GameState::OPTIONS) {
            renderOption(renderer, font, previousState, musicVolume, sfxVolume);
        }
        SDL_Delay(16);
    }
    musicPlayer.cleanup();
    Mix_CloseAudio();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
