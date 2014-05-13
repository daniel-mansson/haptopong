#include "pch.h"
#include "Table.h"


Table::Table(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape) :
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
    m_body->setRestitution(0.9f);
	m_body->setUserPointer(this);
	m_body->setFriction(0.01f);
    
    
    //m_body->setCcdMotionThreshold(0.f);
    //m_body->setCcdSweptSphereRadius(0.f);
}

Table::~Table(void)
{
}

void Table::render(float timeStep)
{
    /*
    btTransform transform;
    btMotionState* pState = m_body->getMotionState();
	pState->getWorldTransform(transform);
	m_shape->setLocalPos(Util::Vec(transform.getOrigin()));
    */
}

void Table::updateLogic(float timeStep)
{

}

void Table::updateHaptics(chai3d::cGenericHapticDevicePtr device, float timeStep)
{

}

void Table::onCollision(const btCollisionResult& collision)
{

}

btRigidBody* Table::getBody() const {
    return m_body.get();
}

chai3d::cMultiMesh* Table::getShape() const {
    return m_shape;
}
