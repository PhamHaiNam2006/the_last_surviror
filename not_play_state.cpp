#include "not_play_state.h"
#include "defs.h"
#include <iostream>

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
        "Guide for new player",
        "Press Arrow keys to move",
        "Press Space to attack",
        "Enemies will come in large wave",
        "The game ends after 5 minutes or if the player dies.",
        "Lore",
        "You have been fighting for your life for so long",
        "that you out of healing potion.",
        "While fighting with those creature,",
        "you notice a big creature is coming to your place."
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

    const char* buttonLabels[4] = {"Start Game", "Guide & Lore", "Options", "Quit"};

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

void resetGame(SDL_Rect& playerDest, int& playerHealth, int& playerStamina,
               std::vector<Enemy>& enemies, SDL_Texture* enemyTexture,
               const std::vector<Obstacle>& obstacles, int& wave, int& n,
               Uint32& gameStartTime, bool& gameTimerActive, bool& gameEnded) {
    playerDest = { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 11, 24, 32 };
    playerHealth = 100;
    playerStamina = 100;

    wave = 1;
    n = 5;
    enemies.clear();
    spawnEnemies(enemies, enemyTexture, n, SCREEN_WIDTH, SCREEN_HEIGHT, playerDest, obstacles, 100);
    for (auto& e : enemies) e.setAllEnemies(&enemies);

    gameStartTime = SDL_GetTicks();
    gameTimerActive = true;
    gameEnded = false;
}
