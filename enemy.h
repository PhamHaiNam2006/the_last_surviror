#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>
#include <vector>
#include "obstacle.h"

class Enemy {
public:
    Enemy(SDL_Texture* tex, int x, int y, int heal);
    void update(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles);
    void render(SDL_Renderer* renderer);
    SDL_Rect getAttackBox() const;
    SDL_Rect getRect() const { return rect; }

    bool alive = true;
    bool isAlive() const { return alive; }
    void getDamage(int damage) { health = (health - damage > 0) ? health - damage : 0; }

    SDL_Rect getHitbox() const { return rect; }

    bool collidesWithOtherEnemies(const SDL_Rect& rect, const std::vector<Enemy>& others);

    void setAllEnemies(std::vector<Enemy>* enemies) { allEnemies = enemies; }

    bool touchingPlayer(const SDL_Rect& playerRect);

private:
    SDL_Rect rect;
    SDL_Texture* texture;

    SDL_Rect playerLastKnownPos;
    int ATTACK_RANGE = 32;

    enum class State {
        MOVING,
        ATTACKING
    } state;

    int animFrame = 0;
    Uint32 lastAnimTime = 0;
    int frameW = 16, frameH = 15;
    SDL_Rect getAnimSrcRect() const;
    bool facingLeft;
    SDL_Rect attackBox;
    int health;

    Uint32 attackStartTime = 0;

    void moveToward(const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles);

    std::vector<Enemy>* allEnemies = nullptr;
};

void spawnEnemies(std::vector<Enemy>& enemies, SDL_Texture* enemyTexture, int count, int mapWidth, int mapHeight, const SDL_Rect& playerRect, const std::vector<Obstacle>& obstacles, float health);
bool noEnemiesAlive(const std::vector<Enemy>& enemies);

#endif
