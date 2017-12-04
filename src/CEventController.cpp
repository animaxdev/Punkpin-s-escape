#include "CEventController.h"


CEventController::CEventController(std::shared_ptr<CModel> m, CGraphicsController &gc)
{
	this->model = m;
	this->graphicsController = &gc;
    this->_running = true;
}

CEventController::~CEventController()
{
	std::cout << "EVENT controller DESTROYED\n";
}

bool CEventController::handleKeyboard()
{
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
        auto player = model->getPlayerObject();

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_LSHIFT])
		{
            player->setVelocity(2);
		}
		if (currentKeyStates[SDL_SCANCODE_W])
		{
            player->move("UP");
		}
		if (currentKeyStates[SDL_SCANCODE_S])
		{
            player->move("DOWN");
		}
		if (currentKeyStates[SDL_SCANCODE_A])
		{
            player->move("LEFT");
		}
		if (currentKeyStates[SDL_SCANCODE_D])
		{
            player->move("RIGHT");
		}
		if (currentKeyStates[SDL_SCANCODE_R])
		{
			model->reset();
		}
        if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
            _running = false;
		}
        if (currentKeyStates[SDL_SCANCODE_P])
		{
            model->togglePause();
			graphicsController->toggleMenu();
		}
        if (currentKeyStates[SDL_SCANCODE_RETURN])
        {
            if(model->isPaused())
                model->togglePause();
        }
        if (currentKeyStates[SDL_SCANCODE_TAB])
        {
            model->getPlayerObject()->switchWeapon();
        }

        if(EDITOR_ENABLED == 1)
        {
            CTools::getInstance().enableLevelEditor_(
                        currentKeyStates,
                        mouseX,
                        mouseY,
                        model->getMapObject()->getMapTiles(),
                        model->getLevelNr()
                        );
        }


	}

	if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{
        auto player = model->getPlayerObject();

		switch (event.key.keysym.sym)
		{
        case SDLK_w:
        case SDLK_s: player->move("NONV");
			break;					//no vertical movement
		case SDLK_a:
        case SDLK_d: player->move("NONH");
			break;					//no horizontal movement
        case SDLK_LSHIFT: player->setVelocity(4);
			break;
           default:
            break;
		}
	}

	return true;
}

bool CEventController::handleMouse()
{ 
    // Get mouse position
    SDL_GetMouseState(&mouseX, &mouseY);
	
    mouseX += graphicsController->getCameraPos().x;
    mouseY += graphicsController->getCameraPos().y;

	//  Calculate player angle relative to player
	//	and camera position relative to current view
    model->getPlayerObject()->calculateAngle(mouseX - 16,
                                            mouseY - 16);

    //  Shoot on mouseclick
    if (event.type == SDL_MOUSEBUTTONDOWN && !model->isPaused())
	{
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            model->addBullet(BULLET);
        }
	}

	return true;
}


bool CEventController::handleInput()
{
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			_running = false;
			return _running;
		}

		handleMouse();
		handleKeyboard();
	}
	return _running;
}
