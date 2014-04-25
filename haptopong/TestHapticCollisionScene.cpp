#include "pch.h"
#include "TestHapticCollisionScene.h"

#include "Application.h"
#include "LinearResponse.h"

using namespace chai3d;

TestHapticCollisionScene::TestHapticCollisionScene(Application& app) :
	Scene(app),
	m_collision(nullptr)
{
	m_hapticDevice = m_app.getHapticDevice();

	m_world = cWorldPtr(new cWorld());
    m_world->m_backgroundColor.set(0.3f, 0.7f, 0.9f);  
	
	m_camera = new cCamera(m_world.get());
    m_world->addChild(m_camera);

	m_font = cFontPtr(NEW_CFONTCALIBRI32());

	cLabel* line;

	line = new cLabel(m_font.get());
	line->setString("1. Linear, Front, No spin");
	m_labels.push_back(line);

	line = new cLabel(m_font.get());
	line->setString("2. Linear, Front, Spin");
	m_labels.push_back(line);

	m_maxLabelWidth = 0;
	for(auto* label : m_labels) 
	{
		m_camera->m_frontLayer->addChild(label);
		m_maxLabelWidth = cMax(m_maxLabelWidth, label->getStringWidth());
	}
}

TestHapticCollisionScene::~TestHapticCollisionScene(void)
{

}

void TestHapticCollisionScene::enter(ScenePtr from)
{

}

void TestHapticCollisionScene::exit(ScenePtr to)
{

}

void TestHapticCollisionScene::render(const double& timeStep)
{
	double w = (double)m_app.getWindowWidth();
	double y = (double)m_app.getWindowHeight();
	y -= 10;

	for(auto* label : m_labels)
	{
		double lh = label->getStringHeight();
		
		y -= lh * 1.2;
		label->setLocalPos(0.5 * (w - m_maxLabelWidth), y, 0);
	}

	m_camera->renderView(m_app.getWindowWidth(), m_app.getWindowHeight());
}

void TestHapticCollisionScene::updateLogic(const double& timeStep)
{
	if(m_collision != nullptr && m_collision->isDone())
	{
		m_collision = nullptr;
	}
}

void TestHapticCollisionScene::updateHaptics(const double& timeStep)
{	
	CollisionResponsePtr collision = m_collision;
	cVector3d force;

	if(collision != nullptr)
	{
		collision->updateHaptics(timeStep, force);
		m_hapticDevice->setForce(force);
	}
	else 
	{
		force.zero();
		m_hapticDevice->setForce(force);
	}
}

void TestHapticCollisionScene::onKeyDown(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '1':
		{
			m_racket.setNormal(cVector3d(1, 0, 0));
			m_racket.setVelocity(cVector3d(2, 0, 0));

			m_ball.setVelocity(cVector3d(-5, 0, 0));
			m_ball.setAngularVelocity(cQuaternion(0,0,0,1));

			m_collision = CollisionResponsePtr(new LinearResponse(m_racket, m_ball));
		}
		break;
	case '2':
		{
			m_racket.setNormal(cVector3d(1, 0, 0));
			m_racket.setVelocity(cVector3d(2, 0, 0));

			m_ball.setVelocity(cVector3d(-5, 0, 0));
			m_ball.setAngularVelocity(cQuaternion(0,10,0,1));

			m_collision = CollisionResponsePtr(new LinearResponse(m_racket, m_ball));
		}
		break;
	}
}
