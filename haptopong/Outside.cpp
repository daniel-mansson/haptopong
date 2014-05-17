#include "pch.h"
#include "Outside.h"


Outside::Outside(btCollisionShape* collisionShape) 
{
    btTransform startTransform;
	startTransform.setIdentity();
    startTransform.setOrigin(btVector3(0,0,-0.5));
	
    btScalar mass(0.);
        
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = (mass != 0.f);
    
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        collisionShape->calculateLocalInertia(mass,localInertia);
    
    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,collisionShape, localInertia);
    m_body = std::make_shared<btRigidBody>(rbInfo);
    m_body->setRestitution(0.7f);
	m_body->setUserPointer(this);
	m_body->setFriction(0.01f);
    
	/*m_body->setCollisionFlags(m_body->getCollisionFlags() |
		btCollisionObject::CF_NO_CONTACT_RESPONSE|
		btCollisionObject::CF_KINEMATIC_OBJECT);*/
    
    //m_body->setCcdMotionThreshold(0.f);
    //m_body->setCcdSweptSphereRadius(0.f);
}

Outside::~Outside(void)
{
}

void Outside::render(float timeStep)
{

}

void Outside::updateLogic(float timeStep)
{

}

void Outside::updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep)
{

}

void Outside::onCollision(const btCollisionResult& collision)
{

}
