#include "CEventController.h"


CEventController::CEventController(std::shared_ptr<CModel> m, CGraphicsController &gc)
{
	this->model = m;
	this->graphicsController = &gc;
	_running = true;
	_isPaused = false;
}

CEventController::~CEventController()
{
	std::cout << "EVENT controller DESTROYED\n";
}

bool CEventController::handleKeyboard()
{
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
	{
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_LSHIFT])
		{
			model->getPlayerObject()->setVelocity(2);
		}
		if (currentKeyStates[SDL_SCANCODE_W])
		{
			model->getPlayerObject()->move("UP");
		}
		if (currentKeyStates[SDL_SCANCODE_S])
		{
			model->getPlayerObject()->move("DOWN");
		}
		if (currentKeyStates[SDL_SCANCODE_A])
		{
			model->getPlayerObject()->move("LEFT");
		}
		if (currentKeyStates[SDL_SCANCODE_D])
		{
			model->getPlayerObject()->move("RIGHT");
		}
		if (currentKeyStates[SDL_SCANCODE_R])
		{
			model->reset();
		}
		if (currentKeyStates[SDL_SCANCODE_Q])
		{
			//model->addEnemy(true);
		}
		if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			togglePause();
			graphicsController->toggleMenu();
		}

		/*CTools::getInstance().enableLevelEditor(
			currentKeyStates, 
			(mouseX + graphicsController->getCameraPos().x),
			(mouseY + graphicsController->getCameraPos().y),
			model->getMapObject()->getMapTiles(),
			model->getLevelNr()
		);*/

	}

	if (event.type == SDL_KEYUP && event.key.repeat == 0)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_w:
		case SDLK_s: model->getPlayerObject()->move("NONV");
			break;					//no vertical movement
		case SDLK_a:
		case SDLK_d: model->getPlayerObject()->move("NONH");
			break;					//no horizontal movement
		case SDLK_LSHIFT: model->getPlayerObject()->setVelocity(4);
			break;
		}
	}


	return true;
}

bool CEventController::handleMouse()
{ 
	SDL_GetMouseState(&mouseX, &mouseY);			// Get mouse position
	
	
	//  Calculate player angle relative to player
	//	and camera position relative to current view
	//	(mouseX + cameraPos.x = actual mouse position)
	model->getPlayerObject()->calculateAngle(mouseX + graphicsController->getCameraPos().x,
											mouseY + graphicsController->getCameraPos().y);

	if (event.type == SDL_MOUSEBUTTONDOWN && !isPaused())
	{
		if (event.button.button == SDL_BUTTON_LEFT)
			model->addBullet(BULLET);
	}
	return true;
}

void CEventController::togglePause()
{
	_isPaused = !_isPaused;

	//	If you're pausing - pause the Timer too...
	if (_isPaused)			
		CTimer::getInstance().pause();
	else
		CTimer::getInstance().unpause();
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
