#pragma once

#include <SDL.h>
#include "CGraphicsController.h"
#include "CModel.h"


class CEventController final
{
private:
	SDL_Event event;
	bool _isPaused;
	bool _running;
	std::shared_ptr<CModel> model;
	CGraphicsController* graphicsController;
	int mouseX, mouseY;

	//	helpers
	bool handleKeyboard();
	bool handleMouse();
	void togglePause();


public:
	CEventController( std::shared_ptr<CModel>, CGraphicsController & );
	~CEventController();

	//	main input controller function
	bool handleInput();
	const bool getRunning() const { return _running; }
	const bool isPaused() const { return _isPaused; }
};