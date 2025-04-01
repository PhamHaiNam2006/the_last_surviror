#include "character.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class Main_char : public Character{
    private:
        int health=100;
        int speed=5;
    public:
        void updateHealth(int n){
            health-=n;
        }
        bool isGameOver(){
            return health!=0;
        }
        void display();
};
