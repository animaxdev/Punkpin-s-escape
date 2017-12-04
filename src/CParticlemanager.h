#pragma once

#include "SParticle.h"
#include "defines.h"
#include <iterator>



// vector of particles
typedef std::vector<std::shared_ptr<Particle>> particleData;

// struct holding data for particle manager
struct particleSetData
{
    particleData particles;
    std::vector<CTexture*> types;
    const double * ax;
    const double * ay;
    int xMain = 0;
    int yMain = 0;
    int maxIters = 2;
    int radius = 25;
    int iters = 0;
};



class CParticleManager
{

private:

    std::vector<particleSetData> particleSet;
    cameraPosition cameraPos;

    void cleanUp();
    void resetParticles(particleSetData &);
    void setCameraPos(cameraPosition pos);



public:

    CParticleManager();

    ~CParticleManager();

    // shows particles relative to camera position
    void showParticles(cameraPosition);

    // add set from addresses of positions
    void addParticleSet(std::vector<CTexture*> &, const double&, const double&, int maxIt=2, int radius=25);

    // add set by value
    void addParticleSet(std::vector<CTexture*> &, int, int, int maxIt=2, int radius=25);

};
