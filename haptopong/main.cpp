#include "pch.h"
#include "Application.h"

#include "MenuScene.h"
#include "PongScene.h"

int main(int argc, char* argv[])
{
	Application app;

	if(!app.initialize("Haptopong", &argc, argv))
	{
		std::cerr<<"Error initalizing app."<<std::endl;
		return 1;
	}
#ifdef TESTING_NETWORK
	app.pushScene(MenuScene::create(app));
#else
	app.pushScene(PongScene::create(app));
#endif
	app.run();

    return 0;
}
