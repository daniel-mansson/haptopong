#include "pch.h"
#include "PongScene.h"

#include "Application.h"

#include <ode/ode.h>

using namespace chai3d;

PongScene::PongScene(Application& app) :
	Scene(app)
{
	m_world = cWorldPtr(new cWorld());
	m_world->m_backgroundColor.set(0.2f, 0.7f, 0.6f);  

	m_camera = new cCamera(m_world.get());
	m_world->addChild(m_camera);

	//Create physics
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0, 0, -10));


	m_groundShape = new btBoxShape(btVector3(btScalar(0.5),btScalar(0.5),btScalar(0.5)));

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

		//add the body to the dynamics world
		m_dynamicsWorld->addRigidBody(m_groundBody);
	}

	
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

			startTransform.setOrigin(btVector3(0,0,3));
		
			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,m_sphereShape,localInertia);
			m_sphereBody = new btRigidBody(rbInfo);

			m_dynamicsWorld->addRigidBody(m_sphereBody);
	}
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
