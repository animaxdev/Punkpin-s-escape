#pragma once

#include <SDL2/SDL.h>
#include "CGraphicsController.h"
#include "CModel.h"


class CEventController final
{
private:
    SDL_Event event;

    bool _running;
    std::shared_ptr<CModel> model;
    CGraphicsController* graphicsController;
    int mouseX, mouseY;

    //  helpers
    bool handleKeyboard();
    bool handleMouse();


public:
    CEventController( std::shared_ptr<CModel>, CGraphicsController & );
    ~CEventController();

    //  main input controller function
    bool handleInput();
    bool getRunning() const { return _running; }

};
