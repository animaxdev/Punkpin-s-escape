#pragma once

#include "SDL2/SDL.h"
#include "CTexture.h"
#include "defines.h"
#include <vector>
#include <memory>


template <class T>
T randMinMax(T min, T max)
{
    T output = min + (rand() % static_cast<int>(max - min + 1));
    return output;
}


struct Particle
{

private:

    int x,y;
    int frame;

public:

    Particle(int xPos, int yPos, int r) {
        //Set offsets
        x = xPos + randMinMax(-r,r) + TEXTURE_SIZE/2;
        y = yPos + randMinMax(-r,r) + TEXTURE_SIZE/2;

        //Initialize animation
        frame = rand() % 5;
    }

    ~Particle() {

    }

    std::pair<int,int> getPos()
    {
        frame++;
        return {x,y};
    }

    bool isDead()
    {
        if(frame > 10)
        {
            return true;
        }
        return false;
    }
};
