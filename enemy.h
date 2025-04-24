#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>

enum class EnemyState {
    IDLE,
    MOVE,
    ATTACK,
    DIE
};

class Enemy {
public:
    Enemy(SDL_Texture* texture, int x, int y);

    void update(const SDL_Rect& playerRect, const std::vector<SDL_Rect>& obstacles, bool playerMoved);
    void render(SDL_Renderer* renderer, const SDL_Rect& camera);

    void activate();
    bool isActive() const;
    SDL_Rect getHitbox() const;
    bool isCollidingWithPlayer(const SDL_Rect& player) const;

private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect hitbox;
    int frame;
    int frameCounter;
    Uint32 lastFrameTime;
    int frameDelay;
    EnemyState state;
    bool active;
    Uint32 lastActionTime;
    int actionDelay;

    void animate();
    void moveTowards(const SDL_Rect& target);
    bool canSeePlayer(const SDL_Rect& player, const std::vector<SDL_Rect>& obstacles) const;
    SDL_Rect getSpriteClip() const;
};

#endif
