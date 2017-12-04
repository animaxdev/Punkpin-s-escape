#pragma once

#include "defines.h"
#include "CTexture.h"
#include "CWorldObject.h"
#include "CTimer.h"
#include "SDL2/SDL.h"

struct animationData {
    std::string name;
    std::vector<std::shared_ptr<CTexture>> textures;
    Uint32 incrTime = 300;
    int currentFrame = 0;
    int maxFrame = 0;
    Uint32 time = 0;
};

struct staticAnimation {
    std::string name;
    position pos = {0, 0};
    Uint32 animationTime = 0;
    Uint32 time = 0;
};

class CAnimationManager
{
private:

    std::vector<animationData> data;
    std::vector<staticAnimation> staticAnimationsData;
    Uint32 currentTime;

    void setCurrent(animationData &);
    bool isAlive(staticAnimation &);

public:

    CAnimationManager();
    ~CAnimationManager();

    void update();
    void addAnimation(animationData);
    void addStaticAnimation(std::string, position, Uint32 animTime = 0);
    void showStaticAnimations(cameraPosition cam = {0,0});
    const std::shared_ptr<CTexture> getCurrent(std::string);
};
