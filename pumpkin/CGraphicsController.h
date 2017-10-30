#pragma once

#include "CModel.h"
#include "CTexture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <conio.h>
#include <memory>
#include <utility>
#include "defines.h"
#include "CTools.h"


class CGraphicsController final //: std::enable_shared_from_this<CGraphicsController>
{
private:

	int frame;
	//	Custom type containing CTexture pointers and
	//	coresponding strings (filenames) for further use
	//
	typedef std::vector<std::shared_ptr<CTexture>> texture;
	std::pair<texture, std::vector<std::string>> textures;

	struct cameraPosition { int x; int y; };
	cameraPosition cameraPos = { 0, 0 };

	SDL_Window* gWindow = NULL;				//	The window we'll be rendering to
	SDL_Renderer* gRenderer = NULL;			//	Window renderer.

	std::shared_ptr<CModel> model;
	
	TTF_Font* gFont = NULL;
	bool loadMedia();
	bool initialize();
	SDL_Color textColor = { 0, 0, 0, 255 };

	bool renderMap();
	bool renderWorldObjects();
	bool cameraUpdate();
	
	SDL_Rect mainView;						//	Views
	SDL_Rect guiView;						//	needs to be wrapped
	SDL_Rect menuView;						//	TODO

	bool renderMenu = false;

	std::shared_ptr<CTexture> getTextureByName(std::string);

	void renderObject(const std::shared_ptr<CWorldObject> obj, 
						const std::shared_ptr<CTexture> txtr);

	std::shared_ptr<CTexture> hpBar;		//	need to wrap
	std::shared_ptr<CTexture> ground;		//	or place somewhere
	std::shared_ptr<CTexture> wall;			//	TODO
	std::shared_ptr<CTexture> door;			//
	std::shared_ptr<CTexture> levelNum;		//
	std::shared_ptr<CTexture> timeElapsed;	//

public:
	CGraphicsController(std::shared_ptr<CModel>);
	CGraphicsController() = default;
	~CGraphicsController();
	CGraphicsController* getGraphicsController() { return this; }
	//std::shared_ptr<CGraphicsController> getGraphicsControllerr() { return shared_from_this(); }
	bool renderFrame();
	void toggleMenu() { renderMenu = !renderMenu; }
	const cameraPosition getCameraPos() const { return cameraPos; };
};