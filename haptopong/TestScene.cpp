#include "pch.h"
#include "TestScene.h"

#include "Application.h"

using namespace chai3d;

TestScene::TestScene(Application& app) :
	Scene(app)
{
	// create a new world.
    m_world = new cWorld();

    // set the background color of the environment
    m_world->m_backgroundColor.setBlack();

    // create a camera and insert it into the virtual world
    m_camera = new cCamera(m_world);
    m_world->addChild(m_camera);

    // position and orient the camera
    m_camera->set( cVector3d (0.5, 0.0, 0.0),    // camera position (eye)
                 cVector3d (0.0, 0.0, 0.0),    // look at position (target)
                 cVector3d (0.0, 0.0, 1.0));   // direction of the (up) vector

    // set the near and far clipping planes of the camera
    m_camera->setClippingPlanes(0.01, 10.0);

    // set stereo mode
	m_camera->setStereoMode(C_STEREO_DISABLED);

    // set stereo eye separation and focal length (applies only if stereo is enabled)
    m_camera->setStereoEyeSeparation(0.01);
    m_camera->setStereoFocalLength(0.5);

    // set vertical mirrored display mode
    m_camera->setMirrorVertical(false);

    // create a directional light source
    m_light = new cDirectionalLight(m_world);

    // insert light source inside world
    m_world->addChild(m_light);

    // enable light source
    m_light->setEnabled(true);                   

    // define direction of light beam
    m_light->setDir(-1.0, 0.0, 0.0); 

    // create a sphere (cursor) to represent the haptic device
    m_cursor = new cShapeSphere(0.01);

    // insert cursor inside world
    m_world->addChild(m_cursor);
	   
	cFont *font = NEW_CFONTCALIBRI20();
    
    // create a label to display the haptic rate of the simulation
    m_label = new cLabel(font);
    m_camera->m_frontLayer->addChild(m_label);
	 
	m_label->setString ("Hej hej hej hej  ");
	m_label->setLocalPos(30, 30);
}

TestScene::~TestScene(void)
{
}

void TestScene::enter(ScenePtr from)
{
}

void TestScene::exit(ScenePtr to)
{
}

void TestScene::render(const double& timeStep)
{
	m_label->setString ("Hej hej hej hej  " + cStr(m_app.getHapticHz()) + "   " + cStr(1.0 / m_app.getSmoothRenderTimeStep(), 0));

	m_camera->renderView(m_app.getWindowWidth(), m_app.getWindowHeight());
}

void TestScene::updateLogic(const double& timeStep)
{
}

void TestScene::updateHaptics(const double& timeStep)
{
}

void TestScene::onKeyDown(unsigned char key, int x, int y)
{
}

void TestScene::onSpecialDown(int key, int x, int y)
{
}
