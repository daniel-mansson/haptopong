#include "pch.h"
#include "Net.h"


Net::Net(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape) :
m_shape(shape)
{
    btTransform startTransform;
	startTransform.setIdentity();
	
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
    m_body->setRestitution(0.0f);
	m_body->setUserPointer(this);
}

Net::~Net(void)
{
}

void Net::render(float timeStep)
{
    /*
    btTransform transform;
    btMotionState* pState = m_body->getMotionState();
	pState->getWorldTransform(transform);
	m_shape->setLocalPos(Util::Vec(transform.getOrigin()));
    */
}

void Net::updateLogic(float timeStep)
{
    
}

void Net::updateHaptics(chai3d::cGenericHapticDevicePtr device, float timeStep)
{
    
}

void Net::onCollision(const btCollisionResult& collision)
{
    
}

btRigidBody* Net::getBody() const {
    return m_body.get();
}

chai3d::cMultiMesh* Net::getShape() const {
    return m_shape;
}
