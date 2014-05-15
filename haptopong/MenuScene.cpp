#include "pch.h"
#include "MenuScene.h"

#include "TestScene.h"
#include "TestHapticCollisionScene.h"
#include "PongScene.h"
#include "NetworkScene.h"
#include "GfxScene.h"

#include "Application.h"

#include "LocalRulesManager.h"
#include "RemoteRulesManager.h"
#include "SimpleCountGameRules.h"

using namespace chai3d;

MenuScene::MenuScene(Application& app) :
	Scene(app)
{
	m_world = cWorldPtr(new cWorld());
	m_world->m_backgroundColor.set(0, 0.1f, 0.3f);  

	m_camera = new cCamera(m_world.get());
	m_world->addChild(m_camera);

	cFontPtr titleFont		= cFontPtr(NEW_CFONTCALIBRI72());
	cFontPtr subtitleFont	= cFontPtr(NEW_CFONTCALIBRI18());
	cFontPtr menuFont		= cFontPtr(NEW_CFONTCALIBRI40());

	m_fonts.push_back(titleFont);
	m_fonts.push_back(subtitleFont);
	m_fonts.push_back(menuFont);

	m_title = new cLabel(titleFont.get());	 
	m_title->setString ("Haptopong");

	m_subtitle = new cLabel(subtitleFont.get());	 
	m_subtitle->setString ("Daniel, Toshi, Mattias");

	cLabel* item;
	item = new cLabel(menuFont.get());
	item->setString("1. Start client");
	m_menu.push_back(item);

	item = new cLabel(menuFont.get());
	item->setString("2. Start server");
	m_menu.push_back(item);

	item = new cLabel(menuFont.get());
	item->setString("3. Start empty debug scene");
	m_menu.push_back(item);

	item = new cLabel(menuFont.get());
	item->setString("4. Exit");
	m_menu.push_back(item);

	m_camera->m_frontLayer->addChild(m_title);
	m_camera->m_frontLayer->addChild(m_subtitle);

	m_maxMenuWidth = 0;
	for(auto* label : m_menu) 
	{
		m_camera->m_frontLayer->addChild(label);
		m_maxMenuWidth = cMax(m_maxMenuWidth, label->getStringWidth());
	}
}

MenuScene::~MenuScene(void)
{
	delete m_camera->m_frontLayer;
}

void MenuScene::enter(ScenePtr from)
{

}

void MenuScene::exit(ScenePtr to)
{

}

void MenuScene::render(const double& timeStep)
{
	double w = (double)m_app.getWindowWidth();
	double h = (double)m_app.getWindowHeight();
	double lw, lh, y = h;

	//Render title
	lw = m_title->getStringWidth();
	lh = m_title->getStringHeight();
	y -= lh * 1.5;
	m_title->setLocalPos(0.5 * (w - lw), y, 0);
	y -= lh * 0.5;

	//Render menu options
	for(auto* label : m_menu)
	{
		lh = label->getStringHeight();

		y -= lh * 1.2;
		label->setLocalPos(0.5 * (w - m_maxMenuWidth), y, 0);
	}

	//Render subtitle
	lw = m_subtitle->getStringWidth();
	lh = m_subtitle->getStringHeight();
	m_subtitle->setLocalPos(5, 5, 0);

	m_camera->renderView(m_app.getWindowWidth(), m_app.getWindowHeight());

#ifdef WIN32
	Sleep(5);
#endif
}

void MenuScene::updateLogic(const double& timeStep)
{

}

void MenuScene::updateHaptics(const double& timeStep)
{
#ifdef WIN32
	Sleep(50);
#endif
}

void MenuScene::onKeyDown(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '1':
		{
			std::string line;
			std::string linePort;
			std::cout<<"Host: ";
			//std::getline(std::cin, line);
			line = "127.0.0.1";
			std::cout<<"Port: ";
			//std::getline(std::cin, linePort);
			linePort = "1234";
			ENetAddress addr; 
			enet_address_set_host (&addr,line.c_str());
			
			std::istringstream(linePort) >> addr.port;

			auto gameRules = GameRulesPtr(new SimpleCountGameRules(20));
			auto metaRules = GameRulesManagerPtr(new RemoteRulesManager(gameRules, addr));
			m_app.pushScene(PongScene::create(m_app, metaRules));
		}
		break;
	case '2':		
		{
			int port = 0;
			std::cout<<"Port: ";
			//std::cin >> port;
			port = 1234;

			auto gameRules = GameRulesPtr(new SimpleCountGameRules(20));
			auto metaRules = GameRulesManagerPtr(new LocalRulesManager(gameRules, port));
			m_app.pushScene(PongScene::create(m_app, metaRules));
		}
		break;
	case '3':
		m_app.pushScene(PongScene::create(m_app));
		break;
	case '4':
		m_app.popScene();
		break;
	}
}

void MenuScene::onSpecialDown(int key, int x, int y)
{
}
