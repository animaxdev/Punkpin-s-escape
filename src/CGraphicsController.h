#pragma once

#include "CModel.h"
#include "CTexture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <ncurses.h>
#include <memory>
#include <utility>
#include "defines.h"
#include "CTools.h"
#include "SParticle.h"
#include "CParticlemanager.h"
#include "CAnimationManager.h"








class CGraphicsController final //: std::enable_shared_from_this<CGraphicsController>
{
private:

    //  Custom type containing CTexture pointers and
    //  coresponding strings (filenames) for further use
    typedef std::vector<std::shared_ptr<CTexture>> texture;
    std::pair<texture, std::vector<std::string>> textures;

    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;

    // managers
    CParticleManager *particleManager;
    CAnimationManager *animationManager;

    // media loader
    bool loadMedia();

    // initializing SDL
    bool initialize();

    // main renderers
    void renderMain();
    void renderGui();
    void renderMenu();

    // helpers
    std::shared_ptr<CTexture> getTextureByName(std::string);
    bool cameraUpdate();
    void addAnimation(std::string name, unsigned n, int ms_per_frame);
    void renderBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor);
    void renderText(int x, int y, std::string txt, SDL_Color color = {0,0,0});
    bool renderMap();
    bool renderWorldObjects();
    void renderObject(const std::shared_ptr<CWorldObject> obj,
        const std::shared_ptr<CTexture> txtr,
        int angleIncluded = -1);

    // views
    SDL_Rect mainView;      //  wrap or place somewhere
    SDL_Rect guiView;       //  TODO
    SDL_Rect menuView;
    SDL_Rect messageView;

    // most used textures
    std::shared_ptr<CTexture> ground;       //  wrap or place somewhere
    std::shared_ptr<CTexture> wall;         //  TODO
    std::shared_ptr<CTexture> door;         //
    std::shared_ptr<CTexture> levelNum;     //
    std::shared_ptr<CTexture> timeElapsed;  //
    std::vector<CTexture*> types;
    std::vector<CTexture*> typesWall;
    std::vector<CTexture*> typesGhost;

    // misc
    int frame;
    cameraPosition cameraPos = { 0, 0 };
    SDL_Color textColor = { 0, 0, 0, 255 };
    TTF_Font* gFont = NULL;
    bool _renderMenu = false;
    std::shared_ptr<CModel> model;


public:

    CGraphicsController(std::shared_ptr<CModel>);
    CGraphicsController() = default;
    ~CGraphicsController();
    CGraphicsController * getGraphicsController() { return this; }
    bool renderFrame();
    void toggleMenu() { _renderMenu = !_renderMenu; }
    const cameraPosition getCameraPos() const { return cameraPos; }
};
