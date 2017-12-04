#include "CParticlemanager.h"



CParticleManager::CParticleManager()
{
    cameraPos = {0,0};
}

CParticleManager::~CParticleManager() {}


void CParticleManager::showParticles(cameraPosition pos)
{
    setCameraPos(pos);
    int alpha;

    for(auto &sets : particleSet)
    {
        resetParticles(sets);

        for(auto &prtcl : sets.particles)
        {
            // shuffle
            int which = rand() % sets.types.size();
            auto pos = prtcl->getPos();
            sets.types[which]->setBlendMode(SDL_BLENDMODE_BLEND);

            // attenuate
            alpha = 255 - (255 * ((float)sets.iters / sets.maxIters));

            sets.types[which]->setAlpha(alpha);
            sets.types[which]->render(pos.first, pos.second,0,randMinMax(-90,90));
        }


        cleanUp();
    }
}

void CParticleManager::addParticleSet(std::vector<CTexture*> & txtrs, const double & x, const double & y, int maxIt, int radius)
{
    particleData tempParticle;

    for(auto &i : txtrs)
    {
        tempParticle.push_back(std::make_shared<Particle>(static_cast<int>(x)-cameraPos.x,
                                                          static_cast<int>(y)-cameraPos.y,
                                                          radius));
    }

    particleSet.push_back({tempParticle,txtrs,&x,&y,0,0,maxIt,radius});
}

void CParticleManager::addParticleSet(std::vector<CTexture*> & txtrs, int posX, int posY, int maxIt, int radius)
{
    particleData tempParticle;

    for(auto &i : txtrs)
    {
        tempParticle.push_back(std::make_shared<Particle>(posX-cameraPos.x,
                                                          posY-cameraPos.y,
                                                          radius));
    }

    particleSet.push_back({tempParticle,txtrs,nullptr,nullptr,posX,posY,maxIt,radius});
}

void CParticleManager::setCameraPos(cameraPosition pos)
{
    cameraPos = pos;
}

void CParticleManager::cleanUp()
{
    auto size = particleSet.size();
    for(decltype(size) i = 0 ; i<size; ++i)
    {
        if(particleSet[i].iters >= particleSet[i].maxIters)
        {
            particleSet.erase(particleSet.begin() + i);
            --size;
        }
    }
}

void CParticleManager::resetParticles(particleSetData &set)
{
    auto size = set.particles.size();
    for(decltype(size) i = 0; i<size; ++i)
    {
        if(set.particles[i]->isDead())
        {
            int new_x;
            int new_y;

            if(set.ax != nullptr)
            {
                new_x = static_cast<int>(*set.ax);
                new_y = static_cast<int>(*set.ay);
            }
            else
            {
                new_x = set.xMain;
                new_y = set.yMain;
            }

            new_x -= cameraPos.x;
            new_y -= cameraPos.y;

            set.particles[i].reset(new Particle(new_x, new_y, set.radius));
            ++set.iters;
        }
    }
}
