#include "CGame.h"
#include <thread>

void init() {
    CGame::getInstance().Start();
}


int main(int argc, char* argv[])
{
    std::thread mainThread(init);
    mainThread.join();
    //CGame::getInstance().Start();
	return 0;
}
