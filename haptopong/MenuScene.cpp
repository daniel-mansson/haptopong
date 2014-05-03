#include "pch.h"
#include "MenuScene.h"

#include "TestScene.h"
#include "TestHapticCollisionScene.h"
#include "PongScene.h"
#include "NetworkScene.h"
#include "GfxScene.h"

#include "Application.h"

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
	item->setString("1. Start TestScene");
	m_menu.push_back(item);
	
	item = new cLabel(menuFont.get());
	item->setString("2. Start PongScene");
	m_menu.push_back(item);

	item = new cLabel(menuFont.get());
	item->setString("3. Start TestHapticCollisionScene");
	m_menu.push_back(item);

	item = new cLabel(menuFont.get());
	item->setString("4. Start NetworkScene");
	m_menu.push_back(item);

	item = new cLabel(menuFont.get());
	item->setString("5. Exit");
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
}

void MenuScene::updateLogic(const double& timeStep)
{

}

void MenuScene::updateHaptics(const double& timeStep)
{

}

void MenuScene::onKeyDown(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '1':
		m_app.pushScene(TestScene::create(m_app));
		break;
	case '2':
		m_app.pushScene(PongScene::create(m_app));
		break;
	case '3':
		m_app.pushScene(TestHapticCollisionScene::create(m_app));
		break;
	case '4':
		//m_app.pushScene(NetworkScene::create(m_app));
        m_app.pushScene(GfxScene::create(m_app));
		break;
	case '5':
		m_app.popScene();
		break;
	}
}
