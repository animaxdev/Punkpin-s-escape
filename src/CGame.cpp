#include "CGame.h"



bool CGame::Initialize()
{
	srand(time(NULL));
	CTimer::getInstance().start();
    CTools::getInstance().loadData();
	return true;
}

bool CGame::Frame()
{
	// If game is not paused
    if (gameModel->isPaused() == false)
	{
		// Updates all model data
		if (!gameModel->worldUpdate())
		{
			std::cout << "gameModel update() failure.\n";
			return false;
		}
	}

	// Handles user input
	if ( !eventController->handleInput() )
	{
		std::cout << "eventController handleInput() failure.\n";
		return false;
	}

	// Renders to window
	if ( !graphicsController->renderFrame() )
	{
		std::cout << "graphicsController render() failure.\n";
		return false;
	}

    // reports some data to console every 30 frames
   if(gameModel->getFrameNr() % 30 == 0)
       gameModel->status();

	return true;
}

bool CGame::ShutDown()
{
	graphicsController.reset();
	eventController.reset();
	gameModel.reset();

	return true;
}

CGame & CGame::getInstance()
{
    static CGame * instance = new CGame();
    return *instance;
}

void CGame::Start(void)
{
	Initialize();
	
	while( eventController->getRunning() )
	{ 
		int start = SDL_GetTicks();

		if ( !Frame() )
		{
			std::cout << "Couldn't process frame!\n";
		}

        // Cap framerate
		int time = SDL_GetTicks() - start;
		if(time < 0) continue;
		int sleepTime = 17 - time;
		if(sleepTime>0)
			SDL_Delay(sleepTime);
	}

	ShutDown();
}


