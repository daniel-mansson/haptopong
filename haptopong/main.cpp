#include "pch.h"
#include "Application.h"

#include "TestScene.h"

int main(int argc, char* argv[])
{
	Application app;

	if(!app.initialize("Haptopong First Test", &argc, argv))
	{
		std::cerr<<"Error initalizing app."<<std::endl;
		return 1;
	}

	app.pushScene(TestScene::create(app));
	
	app.run();
    return 0;
}
