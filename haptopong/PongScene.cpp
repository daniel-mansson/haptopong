#include "pch.h"
#include "PongScene.h"

#include "Application.h"

using namespace chai3d;

PongScene::PongScene(Application& app) :
	Scene(app)
{
	m_world = cWorldPtr(new cWorld());
    m_world->m_backgroundColor.set(0.2f, 0.7f, 0.6f);  
	
	m_camera = new cCamera(m_world.get());
    m_world->addChild(m_camera);
}

PongScene::~PongScene(void)
{

}

void PongScene::enter(ScenePtr from)
{

}

void PongScene::exit(ScenePtr to)
{

}

void PongScene::render(const double& timeStep)
{
	m_camera->renderView(m_app.getWindowWidth(), m_app.getWindowHeight());
}

void PongScene::updateLogic(const double& timeStep)
{

}

void PongScene::updateHaptics(const double& timeStep)
{

}

void PongScene::onKeyDown(unsigned char key, int x, int y)
{

}
