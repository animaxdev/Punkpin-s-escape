#include "CGraphicsController.h"

std::shared_ptr<CTexture> CGraphicsController::getTextureByName(std::string textureName)
{
	ptrdiff_t	pos = distance(textures.second.begin(),
		find(textures.second.begin(), textures.second.end(), textureName));

	return textures.first[pos];
}

void CGraphicsController::renderObject(const std::shared_ptr<CWorldObject> obj, const std::shared_ptr<CTexture> txtr)
{
	txtr->render(static_cast<int>(obj->getX() - TEXTURE_SIZE / 2 - cameraPos.x),
				static_cast<int>(obj->getY() - TEXTURE_SIZE / 2 - cameraPos.y),
				NULL,
				static_cast<int>(obj->getAngle())
				);
}

CGraphicsController::CGraphicsController(std::shared_ptr<CModel> mdl)
{
	this->model = mdl;
	initialize();
	loadMedia();

	guiView.w = 200;
	guiView.x = SCREEN_WIDTH - guiView.w;
	guiView.y = 0;
	guiView.h = 640;

	mainView.x = 0;
	mainView.y = 0;
	mainView.w = SCREEN_WIDTH - guiView.w;
	mainView.h = SCREEN_HEIGHT;

	menuView.w = 200;
	menuView.x = mainView.w/2 - menuView.w/2;
	menuView.y = 120;
	menuView.h = 400;

	frame = 0;
}

CGraphicsController::~CGraphicsController()
{
	std::cout << "graphics controller destroyed." << std::endl;
}

bool CGraphicsController::renderFrame()
{
	SDL_RenderClear(gRenderer);
	cameraUpdate();

	//	Render MAIN
	//
	SDL_RenderSetViewport(gRenderer, &mainView);
	renderMap();
	renderWorldObjects();

	if(frame % 60 < 30)
		renderObject(model->getPlayerObject(), getTextureByName("p1"));
	else
		renderObject(model->getPlayerObject(), getTextureByName("p2"));
	
	//
	//	END render MAIN


	//	render GUI
	//
	SDL_RenderSetViewport(gRenderer, &guiView);
	getTextureByName("gui_background")->render(0, 0, NULL, NULL);

	for (int i = model->getPlayerObject()->getHealthPoints(); i > 0; i--)
	{
		hpBar->render((i) * 16 - 20, 150);
	}
	
	levelNum = std::make_shared<CTexture>(gRenderer, gFont, "LEVEL: " + std::to_string(model->getLevelNr()), textColor);
	levelNum->render(20, 250);
	timeElapsed = std::make_shared<CTexture>(gRenderer, gFont, "time: " + std::to_string(CTimer::getInstance().getTicks() / 1000)+" sec", textColor );
	timeElapsed->render(20, 290);
	//
	//	END render GUI


	//  render MENU
	//
	if (renderMenu)
	{
		SDL_RenderSetViewport(gRenderer, &menuView);
		getTextureByName("menu_background")->render(0, 0, NULL, NULL);
	}
	//
	//	END render MENU


	//Update screen
	frame++;
	SDL_RenderPresent(gRenderer);
	return true;
}

bool CGraphicsController::initialize()
{
	//	Initialization flag
	bool success = true;

	//	Initialize SDL
	if (SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//	Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//	Create window
		gWindow = SDL_CreateWindow("Punkpin's escape", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//	Create vsynced renderer for window
			SDL_SetWindowFullscreen(gWindow, SDL_FALSE);
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//	Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//	Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//	Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}
	SDL_SetRenderDrawColor(gRenderer, 0x7f, 0x7f, 0x7f, 0x00);

	return success;
}

bool CGraphicsController::renderMap()
{
	for (auto &i : model->getMapObject()->getMapTiles())
	{
		for (auto &j : i)
		{
			switch (j->getTileType())
			{
			case GROUND: 
				ground->render(j->getX() - cameraPos.x, j->getY() - cameraPos.y);
				break;
			case WALL:	
				wall->render(j->getX() - cameraPos.x, j->getY() - cameraPos.y);
				break;
			case DOOR:
				door->render(j->getX() - cameraPos.x, j->getY() - cameraPos.y);
				break;
			}
		}
	}
	return true;
}

bool CGraphicsController::renderWorldObjects()
{
	for (auto &i : model->getWorldObjects())
	{
		if ( i->is(ENEMY) )
		{
			if(std::static_pointer_cast<CEnemy>(i)->hasPlannedRoute())
				renderObject(i, getTextureByName("ghost"));
			else
				renderObject(i, getTextureByName("enemy"));
		}
		else if ( i->is(BULLET) || i->is(ENEMY_BULLET) )
		{
			renderObject(i, getTextureByName("bullet"));
		}
	}
	return true;
}

bool CGraphicsController::cameraUpdate()
{
	cameraPos.x = (model->getPlayerObject()->getX() + TEXTURE_SIZE / 2) - SCREEN_WIDTH / 2;
	cameraPos.y = (model->getPlayerObject()->getY() + TEXTURE_SIZE / 2) - SCREEN_HEIGHT / 2;

	//	Keep cameraPos in bounds
	if (cameraPos.x < 0)
	{
		cameraPos.x = 0;
	}
	if (cameraPos.y < 0)
	{
		cameraPos.y = 0;
	}
	if (cameraPos.x > LEVEL_WIDTH - mainView.w)
	{
		cameraPos.x = LEVEL_WIDTH - mainView.w;
	}
	if (cameraPos.y > model->getMapObject()->getMapHeight() - mainView.h)
	{
		cameraPos.y = model->getMapObject()->getMapHeight() - mainView.h;
	}

	return true;
}

bool CGraphicsController::loadMedia()
{
	bool success = true;

	//	Open the font
	gFont = TTF_OpenFont("font.ttf", 18);

	if (gFont == NULL)
	{
		printf("Failed to load font:: %s\n", TTF_GetError());
		success = false;
	}

	//	Add all files from /img folder to a vector.
	//	Final format of a pair is <CTexture, std::string>
	//	
	for (auto &i : CTools::getInstance().readFilenames("img"))
	{
		std::string temp = i.path().string();			// format: "img/filename.png"

		//	Add texture to a vector
		textures.first.push_back(std::make_shared<CTexture>(gRenderer, temp));

		//	Add filtered filename to a vector (as a string)
		temp = temp.substr(4, temp.length() - 4);		// -> format: "filename.png"
		temp = temp.substr(0, temp.length() - 4);		// -> format: "filename"
		textures.second.push_back(temp);				//	add string to a vector
	}

	hpBar = getTextureByName("hp");
	ground = getTextureByName("ground");
	wall = getTextureByName("wall");
	door = getTextureByName("door");

	//_getch();
	return success;
}
