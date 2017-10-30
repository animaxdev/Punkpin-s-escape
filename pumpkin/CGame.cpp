#include "CGame.h"

bool CGame::Initialize()
{
	frameNr = 0;
	srand(time(NULL));
	CTimer::getInstance().start();
	return true;
}

bool CGame::Frame()
{
	// If game is not paused
	if (eventController->isPaused() == false)
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
	//(frameNr % 30 == 0) ? gameModel->status() : NULL;
	frameNr++;

	//auto fp = std::bind(&CModel::addEnemy, gameModel);
	//CTimer::getInstance().setCallback(1000, fp, 5 );

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
	static CGame *instance = new CGame();
	return *instance;
}

void CGame::Start(void)
{
	Initialize();
	
	while( eventController->getRunning() )
	{ 
		if ( !Frame() )
		{
			std::cout << "Couldn't process frame!\n";
		}
	}
	ShutDown();
}
