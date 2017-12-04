#pragma once

#include <SDL2/SDL.h>

///////* NOT IMPLEMENTED YET *///////////


class CView
{
private:
	SDL_Rect viewPort;

public:
	CView(int x, int y, int width, int height) {
		viewPort.x = x;
		viewPort.y = y;
		viewPort.w = width;
		viewPort.h = height;
	}

	~CView() {}
};