#include "pch.h"
#include "Racket.h"
#include "BallProperties.h"


Racket::Racket(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape, const RacketProperties &properties, const btTransform &startTransform) :
	m_normal(-1, 0, 0),
	m_velocity(0, 0, 0),
	m_force(0, 0, 0),
    m_shape(shape)
{
    //btScalar mass = m_properties.getWeight();
    btScalar mass(0.f);  //!!!!!!!!!!
	bool isDynamic = (mass != 0.f);
    
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);
	
    m_body = std::make_shared<btRigidBody>(rbInfo);
	m_body->setRestitution((float)m_properties.getRestitution());
	//m_body->setDamping((float)m_properties.getLinDamping(), (float)m_properties.getAngDamping());
	m_body->setUserPointer(this);
	m_body->setCollisionFlags(m_body->getCollisionFlags() |
		btCollisionObject::CF_NO_CONTACT_RESPONSE);
    
	//0.000034 represents volume
	double force = m_velocity*m_velocity*0.000034/2;
}


Racket::~Racket(void)
{
}

void Racket::render(float timeStep)
{
    btTransform transform;
    
    btMotionState* pState = m_body->getMotionState();
	pState->getWorldTransform(transform);
    
	m_shape->setLocalPos(Util::Vec(transform.getOrigin()));
	m_shape->setLocalRot(chai3d::cMatrix3d(Util::Vec(transform.getRotation().getAxis()), transform.getRotation().getAngle()));
}

void Racket::updateLogic(float timeStep)
{

}

void Racket::updateHaptics(chai3d::cGenericHapticDevicePtr device, float timeStep)
{
	chai3d::cVector3d pos;
	device->getPosition(pos);
	
}

void Racket::onCollision(const btCollisionResult& collision)
{

}

btRigidBody* Racket::getBody() const {
    return m_body.get();
}

chai3d::cMultiMesh* Racket::getShape() const {
    return m_shape;
}
