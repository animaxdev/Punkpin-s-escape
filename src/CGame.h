#pragma once

#include <iostream>
#include <vector>
#include "CGraphicsController.h"
#include "CModel.h"
#include "CEventController.h"
#include <ctime>
#include <memory>


class CGame final
{
private:

	bool Initialize();
	bool Frame();
	bool ShutDown();

	// Keeps all data (except media) and processing funcs
	std::shared_ptr<CModel> gameModel = std::make_shared<CModel>();
	
	// Keeps media data and controls rendering
	std::shared_ptr<CGraphicsController> graphicsController =
		std::make_shared<CGraphicsController>( gameModel->getModel() );

	// Handles user input
	std::unique_ptr<CEventController> eventController = 
		std::make_unique<CEventController>( gameModel->getModel(), *graphicsController->getGraphicsController() );
		

public:

	CGame() = default;								// Prevent construction
	CGame(const CGame&) = delete;					// Prevent construction by copying	
	CGame& operator=(const CGame&) = delete;		// Prevent assignment				
    ~CGame() {  }									// Prevent unwanted destruction
	static CGame& getInstance();
    void Start(void);
};


