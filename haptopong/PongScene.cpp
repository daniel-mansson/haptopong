#include "pch.h"
#include "PongScene.h"
#include "Application.h"
#include "ShadowlessMesh.h"
#include "CustomCamera.h"

#include <ode/ode.h>

using namespace chai3d;

BallEventManagerPtr g_ballEventMgr = nullptr;

bool OnContactProcessed(btManifoldPoint& point,void* body0,void* body1)
{
	GameObject* go0 = (GameObject*)((btRigidBody*)body0)->getUserPointer();
	GameObject* go1 = (GameObject*)((btRigidBody*)body1)->getUserPointer();

	if(go1->getType() == BALL)
		std::swap(go0, go1);

	if(go0->getType() == BALL)
	{
		switch(go1->getType())
		{
		case RACKET:
			g_ballEventMgr->OnRacketHit(point, *(Racket*)go1, *(Ball*)go0);
			break;
		case NET:
			g_ballEventMgr->OnNetHit(point, *(Net*)go1, *(Ball*)go0);
			break;
		case TABLE:
			g_ballEventMgr->OnTableHit(point, *(Table*)go1, *(Ball*)go0);
			break;
		case OUTSIDE:
			g_ballEventMgr->OnOutside(point, *(Ball*)go0);
			break;
		}
	}

	//Return value is ignored by bullet in newer versions.
	return true;
}

PongScene::PongScene(Application& app, GameRulesManagerPtr gameRules) :
	Scene(app),
	m_gameRules(gameRules),
	m_networkTimer(1.0 / 20.0)
{
	m_hapticResponseMgr = HapticResponseManagerPtr(new HapticResponseManager());

	m_ballEventMgr = BallEventManagerPtr(new BallEventManager(m_hapticResponseMgr, m_gameRules));
	g_ballEventMgr = m_ballEventMgr;

	// create a new world.
	m_world = std::make_shared<cWorld>();

	// set the background color of the environment
	m_world->m_backgroundColor.setGrayLevel(0.6f);

	createCamera();
	createLight();

	//Create dynamics world, default settings
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, 0, -10));

	createTable();
	createNet();
	createBall();
	createRackets();
	createOutside();

	gContactProcessedCallback = &OnContactProcessed;

	/*
	m_groundShape = new btBoxShape(btVector3(btScalar(2.74*0.5),btScalar(1.52*0.5),btScalar(0.10*0.5)));

	btTransform groundTransform;
	groundTransform.setIdentity();
	{
	btScalar mass(0.);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
	m_groundShape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,m_groundShape,localInertia);
	m_groundBody = new btRigidBody(rbInfo);
	m_groundBody->setRestitution(0.9f);

	//add the body to the dynamics world
	m_dynamicsWorld->addRigidBody(m_groundBody);
	}

	m_ground = new cShapeBox(2.74, 1.52, 0.10);
	m_ground->setEnabled(false);
	m_world->addChild(m_ground);
	*/

	if(m_gameRules != nullptr)
		m_gameRules->initialize();

	/*
	{
	//create a dynamic rigidbody

	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
	btCollisionShape* m_sphereShape = new btSphereShape(btScalar(1.));

	/// Create Dynamic Objects
	btTransform startTransform;
	startTransform.setIdentity();

	btScalar	mass(1.f);

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
	m_sphereShape->calculateLocalInertia(mass,localInertia);

	startTransform.setOrigin(btVector3(0,0,5));

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,m_sphereShape,localInertia);
	m_sphereBody = new btRigidBody(rbInfo);


	m_dynamicsWorld->addRigidBody(m_sphereBody);
	}

	m_sphere = new cShapeSphere(1.0);
	m_world->addChild(m_sphere);*/
}

PongScene::~PongScene(void)
{
	g_ballEventMgr = nullptr;

#ifdef MACOSX
	// osx texture deletion crash fix
	m_world->removeChild(m_ball->getShape());
#endif
}

void PongScene::enter(ScenePtr from)
{

}

void PongScene::exit(ScenePtr to)
{

}

void PongScene::render(const double& timeStep)
{

	btTransform transform;
	//btMotionState* pState = m_sphereBody->getMotionState();
	//pState->getWorldTransform(transform);
	//m_sphere->setLocalPos(Util::Vec(transform.getOrigin()));

	m_table->render((float)timeStep);
	m_net->render((float)timeStep);
	m_ball->render((float)timeStep);
	m_playerRacket->render((float)timeStep);
	m_opponentRacket->render((float)timeStep);

	m_camera->renderView(m_app.getWindowWidth(), m_app.getWindowHeight());

#ifdef TESTING_NETWORK
	::Sleep(5);
#endif
}

void PongScene::updateLogic(const double& timeStep)
{
	if(m_gameRules != nullptr)
	{
		m_gameRules->update(timeStep);

		if(m_networkTimer.update(timeStep) > 0)
		{
			btVector3 pos = m_playerRacket->getPosition();
			m_gameRules->updatePlayerPos(pos);
		}
	}

	m_table->updateLogic((float)timeStep);
	m_net->updateLogic((float)timeStep);
	m_ball->updateLogic((float)timeStep);
	m_playerRacket->updateLogic((float)timeStep);
	m_opponentRacket->updateLogic((float)timeStep);

	//m_dynamicsWorld->stepSimulation((btScalar)timeStep, 10);
	//m_dynamicsWorld->stepSimulation((btScalar)timeStep, 5, btScalar(1.)/btScalar(120.));
	m_dynamicsWorld->stepSimulation((btScalar)timeStep, 10, btScalar(1.)/btScalar(500.));

	//std::cout << "render hz: " << 1/timeStep << std::endl;


	/*	m_camera->set(cVector3d (2.47, (double)m_ball->getBody()->getCenterOfMassPosition().y(), (double)m_ball->getBody()->getCenterOfMassPosition().z() + 0.3),   // camera position (eye)
	cVector3d (0.0, 0.0, 0.01),    // look at position (target)
	cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector
	*/
}

void PongScene::updateHaptics(const double& timeStep)
{
	/*cVector3d pos;
	m_app.getHapticDevice()->getPosition(pos);

	pos *= 100;
	double mag = cClamp((pos.x() + 3.5) * 1.5, 2.0, 20.0);

	m_camera->set( cVector3d (mag * cCosRad(pos.y()* 0.5), mag * cSinRad(pos.y() * 0.5), pos.z() * 1.7),    // camera position (eye)
	cVector3d (0.0, 0.0, 0.0),    // look at position (target)
	cVector3d (0.0, 0.0, 1.0));   // direction of the (up) vector
	*/
	m_playerRacket->updateHaptics(m_app.getHapticDevice(), timeStep);

	cVector3d force(0,0,0);

	m_hapticResponseMgr->updateHaptics(timeStep, force);

	m_app.getHapticDevice()->setForce(force);

#ifdef TESTING_NETWORK
	::Sleep(10);
#endif
}

void PongScene::onNewRound(const Score& score, PlayerId nextServe, PlayerId prevWinner)
{

}

void PongScene::onGameOver(const Score& score, PlayerId winner)
{

}

btVector3 PongScene::invert(const btVector3& vec)
{
	btVector3 res;
	res[0] = -vec[0];
	res[1] = -vec[1];
	res[2] = vec[2];
	return res;
}

void PongScene::updateOpponentPos(const btVector3& position)
{
	btVector3 pos = invert(position);
	m_opponentRacket->setPosition(pos);
}

void PongScene::updateBallState(const btTransform& transform, const btVector3& velocity, const btVector3& angularVelocity)
{
	m_ball->stop();
	m_ball->setPosition(invert(transform.getOrigin()));
	m_ball->setVelocity(invert(velocity));
	m_ball->setAngularVelocity(invert(angularVelocity));
	m_ball->setActive(true);
}

void PongScene::onKeyDown(unsigned char key, int x, int y)
{
	if(key == 'w')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(2, 0, 0.3f));
		m_ball->setVelocity(btVector3(-4, Util::RandRange(-2, 2), 3.3f));
		m_ball->setAngularVelocity(btVector3(0, 0, 300 * m_ball->getVelocity().y()));
		m_ball->setActive(true);
	}
	if(key == 's')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(-2, 0, 0.3f));
		m_ball->setVelocity(btVector3(4, Util::RandRange(-2, 2), 3.3f));
		m_ball->setAngularVelocity(btVector3(0, 0, -300 * m_ball->getVelocity().y()));
		m_ball->setActive(true);
	}
	if(key == 'e')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(2, 0, 0.7f));
		m_ball->setVelocity(btVector3(-4, Util::RandRange(-1, 1), -1.0f));
		m_ball->setAngularVelocity(btVector3(0, 1400, 0));
		m_ball->setActive(true);
	}
	if(key == 'd')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(2, 0, 0.3f));
		m_ball->setVelocity(btVector3(-4, Util::RandRange(-1, 1), 3.3f));
		m_ball->setAngularVelocity(btVector3(0, -300, 0));
		m_ball->setActive(true);
	}
	if(key == 'q')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(2, 0, 0.3f));
		m_ball->setVelocity(btVector3(-2.1f, -0.9f, 4.5f));
		m_ball->setAngularVelocity(btVector3(0, 0, 00 * m_ball->getVelocity().y()));
		m_ball->setActive(true);
	}
	if(key == 'a')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(1.5, 0, 0.1f));
		m_ball->setVelocity(btVector3(-1, Util::RandRange(-0.5f, 0.5f), 2.0f));
		m_ball->setAngularVelocity(btVector3(0, 0, 00 * m_ball->getVelocity().y()));
		m_ball->setActive(true);
	}
	if(key == '<')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(2, 0, 0.3f));
		m_ball->setVelocity(btVector3(Util::RandRange(-2, -1), -0.1f, 2.f));
		m_ball->setAngularVelocity(btVector3(0, 0, 00 * m_ball->getVelocity().y()));
		m_ball->setActive(true);
	}
	if(key == 'z')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(1.27f, 0.65f, 0.1f));
		m_ball->setVelocity(btVector3(0.04f, 0.04f, 0.0));
		m_ball->setAngularVelocity(btVector3(0, 0, 0));
		m_ball->setActive(true);
	}
	if(key == 'r')
	{
		m_ball->stop();
		m_ball->setPosition(btVector3(1.4f, 0.06f, 0.58f));
		m_ball->setVelocity(btVector3(5.f, 0.f, 0.f));
		m_ball->setAngularVelocity(btVector3(0, 0, 0));
		m_ball->setActive(true);
	}
}

void PongScene::onSpecialDown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_DOWN:
		m_camera->set(cVector3d (0.02, -1.4, 0.055),   // camera position (eye)
			cVector3d (0.0, 0.0, 0.055),    // look at position (target)
			cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector
		m_camera->set(cVector3d (-1.5, 0.0, 0.6),   // camera position (eye)
			cVector3d (-2.0, 0.0, 0.6),    // look at position (target)
			cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector
		break;
	case GLUT_KEY_LEFT:
		m_camera->set(cVector3d (1.7, 0.15, 0.6),   // camera position (eye)
			cVector3d (2.0, 0.0, 0.6),    // look at position (target)
			cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector
		break;
	case GLUT_KEY_RIGHT:
		m_camera->set(cVector3d (1.4, 1.2, 0.17),    // camera position (eye)
			cVector3d (1.4, 0.0, 0.01),    // look at position (target)
			cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector
		m_camera->set(cVector3d (1.2, 1.6, 0.4),    // camera position (eye)
			cVector3d (1.2, 0.0, 0.4),    // look at position (target)
			cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector
		break;
	case GLUT_KEY_UP:
		m_camera->set(cVector3d (2.47, 0.0, 0.95),   // camera position (eye)
			cVector3d (0.0, 0.0, 0.01),    // look at position (target)
			cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector
		break;
	}
}

void PongScene::createCamera()
{
	// create a camera and insert it into the virtual world
	m_camera = new CustomCamera(m_world.get());
	m_world->addChild(m_camera);

	// position and orient the camera
	m_camera->set(cVector3d (2.47, 0.0, 0.95),   // camera position (eye)
		cVector3d (0.0, 0.0, 0.01),    // look at position (target)
		cVector3d (0.0, 0.0, 1.0));    // direction of the (up) vector

	// set the near and far clipping planes of the camera
	m_camera->setClippingPlanes(0.01, 100.0);

	// set stereo mode
	m_camera->setStereoMode(C_STEREO_DISABLED);

	// set stereo eye separation and focal length (applies only if stereo is enabled)
	m_camera->setStereoEyeSeparation(0.01);
	m_camera->setStereoFocalLength(0.5);

	// set vertical mirrored display mode
	m_camera->setMirrorVertical(false);

	// enable multi-pass rendering to handle transparent objects
	m_camera->setUseMultipassTransparency(true);

	// enable shadow casting
	m_camera->setUseShadowCasting(true);
}

void PongScene::createLight()
{
	/////////////////////////////////////////////////////////////////////////
	// create a directional light source
	/////////////////////////////////////////////////////////////////////////

	chai3d::cDirectionalLight *dirLight = new cDirectionalLight(m_world.get());

	// insert light source inside world
	m_world->addChild(dirLight);

	// enable light source
	dirLight->setEnabled(true);

	// define direction of light beam
	dirLight->setDir(-7 , 5, -3);

	// set lighting conditions
	dirLight->m_ambient.set(0.4f, 0.4f, 0.4f);
	dirLight->m_diffuse.set(0.45f, 0.45f, 0.45f);
	dirLight->m_specular.set(0.2f, 0.2f, 0.2f);

	//dirLight->setUseTwoSideLightModel(false);

	/////////////////////////////////////////////////////////////////////////
	// create a spot light source
	/////////////////////////////////////////////////////////////////////////

	chai3d::cSpotLight* spotLight = new cSpotLight(m_world.get());

	// attach light to camera
	//m_world->addChild(spotLight);

	// enable light source
	spotLight->setEnabled(true);

	// position the light source
	spotLight->setLocalPos(2.5, 0.0, 2.0);

	// define the direction of the light beam
	spotLight->setDir(-1.0, 0.0, -1.0);

	// enable this light source to generate shadows
	spotLight->setShadowMapEnabled(true);

	// set the resolution of the shadow map
	spotLight->m_shadowMap->setResolutionMedium();

	//spotLight->setShadowMapProperties(3, 10);

	// set light cone half angle
	//spotLight->setCutOffAngleDeg(25);
	spotLight->setCutOffAngleDeg(35);

	// set lighting conditions
	spotLight->m_ambient.set(0.0f, 0.0f, 0.0f);
	spotLight->m_diffuse.set(0.9f, 0.9f, 0.9f);
	spotLight->m_specular.set(0.7f, 0.7f, 0.7f);

	//spotLight->setUseTwoSideLightModel(false);
	//spotLight->setSpotExponent(0);
}

void PongScene::createTable()
{
	/////////////////////////////////////////////////////////////////////////
	// create visual shape
	/////////////////////////////////////////////////////////////////////////

	cMultiMesh* table = new cMultiMesh();

	bool fileload = table->loadFromFile("../gfx/table.obj");

	if (!fileload)
	{
		std::cout << "Error - 3D Model failed to load correctly" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	m_world->addChild(table);

	// enable culling to disable rendering of the inside
	table->setUseCulling(true);

	// enable display list for faster graphic rendering (recompute if translated)
	table->setUseDisplayList(true, true);

	// create texture
	cTexture2dPtr table_texture = cTexture2d::create();
	table_texture->setWrapMode(GL_REPEAT);
	fileload = table_texture->loadFromFile("../gfx/table_diffuse.png");
	if (!fileload)
	{
		std::cout << "Error - Texture image failed to load correctly." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	table->setTexture(table_texture);
	table->setUseTexture(true, true);

	/////////////////////////////////////////////////////////////////////////
	// create physics body
	/////////////////////////////////////////////////////////////////////////

	m_tableCollisionShape = std::shared_ptr<btCollisionShape>(Util::LoadCollisionShape("../gfx/table_body.obj"));
	//m_tableShape = new btBoxShape(btVector3(btScalar(2.74*0.5),btScalar(1.52*0.5),btScalar(0.10*0.5)));

	m_table = std::make_shared<Table>(table, m_tableCollisionShape.get());

	m_dynamicsWorld->addRigidBody(m_table->getBody());
}

void PongScene::createNet()
{
	/////////////////////////////////////////////////////////////////////////
	// create visual shape
	/////////////////////////////////////////////////////////////////////////

	cMultiMesh* net = new cMultiMesh();

	bool fileload = net->loadFromFile("../gfx/net.obj");

	if (!fileload)
	{
		std::cout << "Error - 3D Model failed to load correctly" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	m_world->addChild(net);

	// enable culling to disable rendering of the inside
	net->setUseCulling(true);

	// enable display list for faster graphic rendering (recompute if translated)
	net->setUseDisplayList(true, true);

	// create texture
	cTexture2dPtr net_texture = cTexture2d::create();
	net_texture->setWrapMode(GL_REPEAT);
	fileload = net_texture->loadFromFile("../gfx/net_diffuse.png");
	if (!fileload)
	{
		std::cout << "Error - Texture image failed to load correctly." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	net->getMesh(0)->setTexture(net_texture);
	net->getMesh(0)->setUseTexture(true, true);

	// enable transparency for this object
	net->getMesh(0)->m_texture->m_image->setTransparentColor(30, 30, 30, 0);
	net->getMesh(0)->setUseTransparency(true);

	/////////////////////////////////////////////////////////////////////////
	// create physics body
	/////////////////////////////////////////////////////////////////////////

	m_netCollisionShape = std::shared_ptr<btCollisionShape>(Util::LoadCollisionShape("../gfx/net_body.obj"));

	m_net = std::make_shared<Net>(net, m_netCollisionShape.get());

	m_dynamicsWorld->addRigidBody(m_net->getBody());
}

void PongScene::createBall()
{
	BallProperties properties;

	cShapeSphere* ballShape = new cShapeSphere((double)properties.getRadius());

	m_world->addChild(ballShape);

	m_ballCollisionShape = std::make_shared<btSphereShape>(btScalar(properties.getRadius()));

	cTexture2dPtr net_texture = cTexture2d::create();
	net_texture->setWrapMode(GL_REPEAT);
	bool fileload = net_texture->loadFromFile("../gfx/ball_diffuse.png");
	if (!fileload)
	{
		std::cout << "Error - Texture image failed to load correctly." << std::endl;
		std::exit(EXIT_FAILURE);
	}

	ballShape->setTexture(net_texture);
	ballShape->setUseTexture(true, true);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0,0,1));
	//startTransform.setOrigin(btVector3(1.75,0,0.65));
	//startTransform.setOrigin(btVector3(2.2,0,0.9));

	m_ball = std::make_shared<Ball>(ballShape, m_ballCollisionShape.get(), properties, startTransform);

	m_dynamicsWorld->addRigidBody(m_ball->getBody());
}


void PongScene::createOutside()
{
	BallProperties properties;

	m_outsideCollisionShape = btCollisionShapePtr(new btBoxShape(btVector3(50, 50, 0.2f))); 


	m_outside = std::make_shared<Outside>(m_outsideCollisionShape.get());

	m_dynamicsWorld->addRigidBody(m_outside->getBody().get());
}

void PongScene::createRackets()
{
    /////////////////////////////////////////////////////////////////////////
    // create visual shapes
    /////////////////////////////////////////////////////////////////////////
    
    // player racket
    
    //cMultiMesh* playerRacket = new cMultiMesh();
    ShadowlessMesh* playerRacket = new ShadowlessMesh();
    
    bool fileload = playerRacket->loadFromFile("../gfx/racket.obj");
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    cMaterial mat;
    mat.m_ambient.set( 0.2f, 0.2f, 0.2f);
    mat.m_diffuse.set( 1.0f, 1.0f, 1.0f);
    mat.m_specular.set(1.0f, 1.0f, 1.0f);
    playerRacket->setMaterial(mat, true);
    //playerRacket->computeAllNormals();
    
    playerRacket->setUseTransparency(true);
    playerRacket->setTransparencyLevel(0.6f);
    
    m_world->addChild(playerRacket);
    
    // opponent racket
    
    //ShadowlessMesh* opponentRacket = playerRacket->copy(false, false, true);
    //ShadowlessMesh* opponentRacket = new ShadowlessMesh();
    cMultiMesh* opponentRacket = new cMultiMesh();
    
    fileload = opponentRacket->loadFromFile("../gfx/racket.obj");
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    mat;
    mat.m_ambient.set( 0.5f, 0.5f, 0.5f);
    mat.m_diffuse.set( 0.5f, 0.5f, 0.5f);
    mat.m_specular.set(1.0f, 1.0f, 1.0f);
    opponentRacket->setMaterial(mat, true);
    //opponentRacket->computeAllNormals();
    
    opponentRacket->setUseCulling(true);
    
    m_world->addChild(opponentRacket);
    
    
    /////////////////////////////////////////////////////////////////////////
    // create physics bodys
    /////////////////////////////////////////////////////////////////////////
    
    // player racket
    
    RacketProperties properties;
    
    m_racketsCollisionShape = std::shared_ptr<btCollisionShape>(Util::LoadCollisionShape("../gfx/racket_body.obj"));
    
    btTransform startTransform;
	startTransform.setIdentity();
	//startTransform.setOrigin(btVector3(2.1f, 0, 0.88f));
	startTransform.setOrigin(btVector3(1.9f, 0, 0.6f));
	startTransform.setRotation(btQuaternion(0, 40*0.0174532925f, 0));

	m_playerRacket = std::make_shared<Racket>(playerRacket, m_racketsCollisionShape.get(), properties, startTransform);
	if(m_gameRules != nullptr)
		m_playerRacket->setPlayerId(m_gameRules->getPlayerId());

	m_dynamicsWorld->addRigidBody(m_playerRacket->getBody());

	// opponent racket

	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(-1.9f, 0, 0.6f));
	startTransform.setRotation(btQuaternion(0, -40*0.0174532925f, 0));

	m_opponentRacket = std::make_shared<Racket>(opponentRacket, m_racketsCollisionShape.get(), properties, startTransform);
	if(m_gameRules != nullptr)
		m_opponentRacket->setPlayerId(m_gameRules->getOpponentId());

	m_dynamicsWorld->addRigidBody(m_opponentRacket->getBody());


}


