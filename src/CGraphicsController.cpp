#include "CGraphicsController.h"

std::shared_ptr<CTexture> CGraphicsController::getTextureByName(std::string textureName)
{
	ptrdiff_t	pos = distance(textures.second.begin(),
		find(textures.second.begin(), textures.second.end(), textureName));

	return textures.first[pos];
}

void CGraphicsController::renderObject(const std::shared_ptr<CWorldObject> obj, const std::shared_ptr<CTexture> txtr, int angleIncluded)
{
    txtr->render(static_cast<int>(obj->getX() - cameraPos.x),
                static_cast<int>(obj->getY() - cameraPos.y),
				NULL,
                angleIncluded == -1 ? static_cast<int>(obj->getAngle()) : angleIncluded
				);
}

CGraphicsController::CGraphicsController(std::shared_ptr<CModel> mdl)
{
	initialize();
	loadMedia();
    this->model = mdl;

	guiView.w = 200;
	guiView.x = SCREEN_WIDTH - guiView.w;
	guiView.y = 0;
    guiView.h = 768;

	mainView.x = 0;
	mainView.y = 0;
	mainView.w = SCREEN_WIDTH - guiView.w;
	mainView.h = SCREEN_HEIGHT;

	menuView.w = 200;
	menuView.x = mainView.w/2 - menuView.w/2;
	menuView.y = 120;
	menuView.h = 400;

    messageView.w = 460;
    messageView.x = mainView.w/2 - messageView.w/2;
    messageView.h = 310;
    messageView.y = SCREEN_HEIGHT/2 - messageView.h/2;

	frame = 0;
}

CGraphicsController::~CGraphicsController()
{
	std::cout << "graphics controller destroyed." << std::endl;
    delete particleManager;
    delete animationManager;
}

bool CGraphicsController::renderFrame()
{
	SDL_RenderClear(gRenderer);

    // Keep it in bounds
    this->cameraUpdate();
    this->animationManager->update();

    // Main renderers
    this->renderMain();
    this->renderGui();
    this->renderMenu();

    // Update screen
	SDL_RenderPresent(gRenderer);
    ++frame;
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
//    SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_SetRenderDrawColor(gRenderer, 0x7f, 0x7f, 0x7f, 0x00);
    SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);

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

            if(j->isHit())
            {
                particleManager->addParticleSet(typesWall,
                                                j->getAddrX(),
                                                j->getAddrY(),4,40
                                                );
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
            auto enemy = std::dynamic_pointer_cast<CEnemy>(i);
            auto x = enemy->getX() - cameraPos.x;
            auto y = enemy->getY() - cameraPos.y - 10;


            if(enemy->getHp() < HP_BAR_TRESHOLD)
            {
                renderBar(x,y,TEXTURE_SIZE,5,enemy->getPercentHp(),{0xff,0,0,0x99}, {50,50,50,0x55});
            }

            if(enemy->getName() == "ghost" && model->getFrameNr() % 3  == 0)
            {
                particleManager->addParticleSet(typesGhost,
                                                (int)i->getX(),
                                                (int)i->getY(),4,10
                                                );
            }

            if(i->isHit())
            {
                particleManager->addParticleSet(types,
                                                enemy->getAddrX(),
                                                enemy->getAddrY(),8
                                                );

                if(i->isDead == true)
                {
                    if(enemy->getHp() > 20)
                    {
                        animationManager->addStaticAnimation("explosion", {i->getX()+100,i->getY()+100},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX()-100,i->getY()-100},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX(),i->getY()+100},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX(),i->getY()-100},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX()+100,i->getY()},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX()-100,i->getY()},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX()+100,i->getY()},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX(),i->getY()-100},3200);
                        animationManager->addStaticAnimation("explosion", {i->getX(),i->getY()},3200);
                    }
                    else if(enemy->getHp() > 0)
                    {
                        animationManager->addStaticAnimation("tombstone", {i->getX(),i->getY()}, 4000);
                    }
                }

               //animationManager->addStaticAnimation("fire", {j->getX(),j->getY()},1000);
            }

//            static int ran;
//            if(enemy->isHit()){
//                enemy->resetTimer();
//                ran = rand() % enemyMessages.size();
//            }

//            if(enemy->spawnTime < 4000)
//            {
//                renderText(i->getX() - cameraPos.x-PTEXT_POS,
//                           i->getY() - cameraPos.y-PTEXT_POS,
//                           enemyMessages[ran], {0,0xff,30,
//                                                ( 255 - ((float)enemy->spawnTime/(float)4000) * 255) });
//            }

            //renderObject(i, getTextureByName(enemy->getName()));

            //animateObject(i,zombies);
            auto name = enemy->getName();
            auto currentFrame = animationManager->getCurrent(name);
            if(currentFrame != nullptr)
                renderObject(i, currentFrame, enemy->getAngleToRender());
            else
                renderObject(i, getTextureByName(name), enemy->getAngleToRender());

		}
        else if ( i->is(BULLET) )
		{
			renderObject(i, getTextureByName("bullet"));
		}
        else if( i->is(ENEMY_BULLET) )
        {
            renderObject(i, animationManager->getCurrent("fireball"));
        }
        else if( i->is(ITEM))
        {
            auto item = std::dynamic_pointer_cast<CItem>(i);

            auto name = item->getName();
            auto currentFrame = animationManager->getCurrent(name);
            if(currentFrame != nullptr)
                renderObject(i, currentFrame);
            else
                renderObject(i, getTextureByName(name));
        }
	}
	return true;
}

bool CGraphicsController::cameraUpdate()
{
    cameraPos.x = model->getPlayerObject()->getX() - SCREEN_WIDTH / 2;
    cameraPos.y = model->getPlayerObject()->getY() - SCREEN_HEIGHT / 2;

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
    if (cameraPos.y > LEVEL_HEIGHT - mainView.h)
	{
        cameraPos.y = LEVEL_HEIGHT - mainView.h;
	}

	return true;
}

bool CGraphicsController::loadMedia()
{
	bool success = true;

	//	Open the font
    gFont = TTF_OpenFont("data/font.ttf", 22);

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

	ground = getTextureByName("ground");
	wall = getTextureByName("wall");
	door = getTextureByName("door");

    types = {{new CTexture(gRenderer, "img//particles//1.png" ),
             new CTexture(gRenderer, "img//particles//2.png" ),
             new CTexture(gRenderer, "img//particles//3.png" )}};

    typesWall = {{new CTexture(gRenderer, "img//particles//wall1.png" ),
             new CTexture(gRenderer, "img//particles//wall2.png" )}};

    typesGhost =  {{new CTexture(gRenderer, "img//particles//ghost1.png" ),
                    new CTexture(gRenderer, "img//particles//ghost2.png" )}};


    particleManager = new CParticleManager();
    animationManager = new CAnimationManager();

    addAnimation("ghost",2, 200);
    addAnimation("zombie", 8, 100);
    addAnimation("fire", 6, 50);
    addAnimation("badpumpkin", 10, 100);
    addAnimation("player", 2, 300);
    addAnimation("angryghost", 8, 250);
    addAnimation("fireball", 8, 150);
    addAnimation("explosion", 16 , 50);
    addAnimation("tombstone", 1 , 8000);
    addAnimation("point", 4 , 150);

	return success;
}

void CGraphicsController::renderBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor)
{
    Percent = Percent > 1.f ? 1.f : Percent < 0.f ? 0.f : Percent;
    SDL_Color old;
    SDL_GetRenderDrawColor(gRenderer, &old.r, &old.g, &old.g, &old.a);
    SDL_Rect bgrect = { x, y, w, h };
    SDL_SetRenderDrawColor(gRenderer, BGColor.r, BGColor.g, BGColor.b, BGColor.a);
    SDL_RenderFillRect(gRenderer, &bgrect);
    SDL_SetRenderDrawColor(gRenderer, FGColor.r, FGColor.g, FGColor.b, FGColor.a);
    int pw = static_cast<int>((float)w * Percent);
    SDL_Rect fgrect = { x, y, pw, h };  
    SDL_RenderFillRect(gRenderer, &fgrect);
    SDL_SetRenderDrawColor(gRenderer, old.r, old.g, old.b, old.a);
}

void CGraphicsController::renderText(int x, int y, std::string txt, SDL_Color color)
{
    std::string tempstr;

    int j = 0;

    for(int i = 0; i != txt.length()+1; i++)
    {
        if(txt[i] == '/' || txt[i] == '\0')
        {
            if(tempstr.empty() == false) {
                CTexture txt(gRenderer, gFont, tempstr, color);

                if(color.a != NULL)
                {
                    txt.setBlendMode(SDL_BLENDMODE_BLEND);
                    txt.setAlpha(color.a);
                }

                txt.render(x,y+j*NEWLINE_SIZE);
                tempstr.clear();
            }
            ++j;
        }
        else
            tempstr += txt[i];
    }
}

void CGraphicsController::renderMain()
{
    SDL_RenderSetViewport(gRenderer, &mainView);
    static int alpha = 0;

    renderMap();
    animationManager->showStaticAnimations(cameraPos);
    
    renderWorldObjects();


    auto player = model->getPlayerObject();
    static int t, ran;

    if( player->isHit() )
    {
        t = frame+254;
        ran = rand() % playerMessages.size();

        particleManager->addParticleSet(types,
                                        player->getAddrX(),
                                        player->getAddrY()
                                        );
        alpha = 100;
    }

    if(t != frame && t != 0)
    {
        renderText(player->getX() - cameraPos.x-PTEXT_POS,
                   player->getY() - cameraPos.y-PTEXT_POS,
                   playerMessages[ran], {0x66,0xff,0x00,255-(frame-t)});
    }
    else
        t = 0;

    renderObject(player, animationManager->getCurrent("player"));

    static int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    getTextureByName("crosshair")->render(mouseX,mouseY);

	particleManager->showParticles(cameraPos);

    if(alpha > 0)
    {
        renderBar(0,0,mainView.w,mainView.h, 100, {255,0,0,alpha}, {0,0,0,0});
        if(alpha > 0)
            alpha-=4;
    }
}

void CGraphicsController::renderGui()
{
    SDL_RenderSetViewport(gRenderer, &guiView);
    getTextureByName("gui_background")->render(0, 0, NULL, NULL);

    auto player = model->getPlayerObject();

    auto percentHp = ( player->getHealthPoints() / player->getMaxHp() ) ;
    renderBar(20,150,100,20,percentHp,{0xff,0,0,0x99},{50,50,50,0x55});


    auto weapon = player->getActiveWeapon();
    if(weapon)
    {
        renderText(20, 190, weapon->getWeaponName());

        renderBar(20, 220, 100, 20, weapon->getReloadProgress(),
                                    {0xff,0xff,0,0xff},
                                    {50,50,50,0x55});

        renderText(130, 220, std::to_string(weapon->getBulletCount()) +
                                      " of " + std::to_string(weapon->getBulletCountMax()));

        renderText(20, 250, "dmg: " + std::to_string(weapon->getWeaponDamage()));
    }

    renderText(20,350,"level: " + std::to_string(model->getLevelNr()));
    renderText(20,390,"time: " + std::to_string(CTimer::getInstance().getTicks() / 1000)+" sec");
    renderText(20,430,"score: " + std::to_string(player->getScore()));

    //  event message render
    if (!model->getCurrentMessage().empty())
    {
        SDL_RenderSetViewport(gRenderer, &messageView);
        getTextureByName("msg_background")->render(0, 0, NULL, NULL);
        renderText(50,50,model->getCurrentMessage());
    }
}

void CGraphicsController::renderMenu()
{
    if (_renderMenu)
    {
        SDL_RenderSetViewport(gRenderer, &menuView);
        getTextureByName("menu_background")->render(0, 0, NULL, NULL);
    }
}

void CGraphicsController::addAnimation(std::string name, unsigned n, int ms_per_frame)
{
    std::vector<std::shared_ptr<CTexture>> temp;
    for(int i = 0; i<n; ++i)
    {
        temp.push_back(getTextureByName(name+std::to_string(i+1)));
    }

    animationManager->addAnimation({name,temp,ms_per_frame});
}
