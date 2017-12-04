#include "CAnimationManager.h"


CAnimationManager::CAnimationManager() {
    currentTime = SDL_GetTicks();
}

CAnimationManager::~CAnimationManager() { }


void CAnimationManager::setCurrent(animationData & dat)
{
    if(currentTime > (dat.time+dat.incrTime))
    {
        dat.time = currentTime;

        if(++dat.currentFrame >= dat.maxFrame)
            dat.currentFrame = 0;
    }
}

void CAnimationManager::update()
{
    currentTime = SDL_GetTicks();
}

void CAnimationManager::addAnimation(animationData animation)
{
    animation.time = SDL_GetTicks();
    animation.maxFrame = animation.textures.size();
    data.push_back(animation);
}

const std::shared_ptr<CTexture> CAnimationManager::getCurrent(std::string name)
{
    for(auto &i : data)
    {
        if(i.name == name)
        {
            setCurrent(i);
            return i.textures[i.currentFrame];
        }
    }

//    auto size = data.end();
//    for(auto i = data.begin(); i != size; ++i)
//    {
//        if(i->name == name) {
//            setCurrent(*i);

//            if(isAlive(i) == false) {
//                data.erase(i);
//                --i;
//            }
//            return i->textures[i->currentFrame];
//        }
//    }
    return nullptr;
}

void CAnimationManager::addStaticAnimation(std::string name, position pos, Uint32 animTime)
{
    //Uint32 t = currentTime;
    staticAnimation tempStaticAnimationData = {name,pos,animTime,currentTime};
    this->staticAnimationsData.push_back(tempStaticAnimationData);
}

void CAnimationManager::showStaticAnimations(cameraPosition cam)
{
//    for(auto &i : staticAnimationsData)
//    {
//        if(isAlive(i))
//        {
//            getCurrent(i.name)->render(i.pos.first - cam.x, i.pos.second - cam.y);
//            i.time = this->currentTime;
//        }
//    }
        auto size = staticAnimationsData.size();
        for(decltype(size) i = 0; i < size; ++i)
        {
            if(isAlive(staticAnimationsData[i]))
            {
                auto currentTexture = getCurrent(staticAnimationsData[i].name);
                int t_diff = (staticAnimationsData[i].animationTime  + staticAnimationsData[i].time) - currentTime;

                if(t_diff <= 255)
                    currentTexture->setAlpha(t_diff);
                else
                    currentTexture->setAlpha(255);

                currentTexture->render(staticAnimationsData[i].pos.first - cam.x, staticAnimationsData[i].pos.second - cam.y);
            }
            else
            {
                staticAnimationsData.erase(staticAnimationsData.begin()+i);
                --size;
            }
        }
}

bool CAnimationManager::isAlive(staticAnimation & dat)
{
    if(dat.animationTime == 0)
        return true;
    else if(currentTime >= (dat.time + dat.animationTime))
        return false;
    else
        return true;
}
