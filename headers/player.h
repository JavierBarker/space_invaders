#pragma once
#include "raylib.h"

class Player {
    private:
    Rectangle player;

    float normalSpeed;
    float slowSpeed;

    bool slowMode;
    
    public:
    Player();

    void Update(float time);

    void Draw(float time);

};