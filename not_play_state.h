#ifndef NOT_PLAY_STATE_H
#define NOT_PLAY_STATE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "enemy.h"
#include "obstacle.h"

enum class GameState {
    MENU,
    TUTORIAL,
    OPTIONS,
    QUIT,
    PLAYING
};

void RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color);
void renderOption(SDL_Renderer* renderer, TTF_Font* font, GameState gameState, int musicVol, int sfxVol);
void renderTutorialScreen(SDL_Renderer* renderer, TTF_Font* font);
void renderStartScreen(SDL_Renderer* renderer, TTF_Font* font, TTF_Font* titleFont, SDL_Texture* menuBackground);

void resetGame(SDL_Rect& playerDest, int& playerHealth, int& playerStamina,
               std::vector<Enemy>& enemies, SDL_Texture* enemyTexture,
               const std::vector<Obstacle>& obstacles, int& wave, int& n, int& point,
               Uint32& gameStartTime, bool& gameTimerActive, bool& gameEnded);

#endif
