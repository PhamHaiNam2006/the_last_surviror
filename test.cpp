#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 128;
const int SPRITE_WIDTH = 64;
const int SPRITE_HEIGHT = 64;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* walkTexture = nullptr;
SDL_Texture* attackTexture = nullptr;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("SDL Character Animation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

SDL_Texture* loadTexture(const char* path) {
    SDL_Surface* surface = IMG_Load(path);
    if (!surface) {
        std::cerr << "Unable to load image " << path << "! IMG_Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void close() {
    SDL_DestroyTexture(walkTexture);
    SDL_DestroyTexture(attackTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* args[]) {
    if (!init()) return 1;

    walkTexture = loadTexture("E:/learning_material/code_blocks/dungeon_explorer/png_file/main_char/sword_walk.png");
    attackTexture = loadTexture("E:/learning_material/code_blocks/dungeon_explorer/png_file/main_char/sword_fight.png");
    if (!walkTexture || !attackTexture) return 1;

    bool quit = false;
    SDL_Event e;
    int x = 50;
    bool attacking = false;
    int frame = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT: x -= 5; break;
                    case SDLK_RIGHT: x += 5; break;
                    case SDLK_SPACE: attacking = true; frame = 0; break;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_Rect srcRect = { frame * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT };
        SDL_Rect dstRect = { x, SCREEN_HEIGHT / 2, SPRITE_WIDTH, SPRITE_HEIGHT };

        if (attacking) {
            SDL_RenderCopy(renderer, attackTexture, &srcRect, &dstRect);
            frame++;
            if (frame >= 3) attacking = false;
        } else {
            SDL_RenderCopy(renderer, walkTexture, &srcRect, &dstRect);
            frame = (frame + 1) % 1;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    close();
    return 0;
}
