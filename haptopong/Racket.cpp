#include "pch.h"
#include "Racket.h"
#include "BallProperties.h"


Racket::Racket(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape, const RacketProperties &properties, const btTransform &startTransform) :
	m_normal(-1, 0, 0),
	m_velocity(0, 0, 0),
	m_force(0, 0, 0),
    m_shape(shape),
	m_hapticPos(0,0,0),
	m_moveAreaScale(10.0),
	m_playerId(NO_PLAYER)
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
		btCollisionObject::CF_NO_CONTACT_RESPONSE|
		btCollisionObject::CF_KINEMATIC_OBJECT);
	m_body->setActivationState(DISABLE_DEACTIVATION);
    
	//0.000034 represents volume
	double force = m_velocity*m_velocity*0.000034/2;

	m_startPos = Util::Vec(m_body->getCenterOfMassPosition());

	m_body->getMotionState()->getWorldTransform(m_transform);
	m_origin = m_transform.getOrigin();
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
	chai3d::cVector3d offset = m_hapticPos;
	offset *= m_moveAreaScale;
	
	m_transform.setOrigin(Util::Vec(offset) + m_origin);
	m_body->getMotionState()->setWorldTransform(m_transform);
}

void Racket::updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep)
{
#ifdef USE_MOUSE
	
	chai3d::cVector3d prev = m_hapticPos;
	
	POINT p;
	GetCursorPos(&p);

	m_hapticPos(0) = 0.0;
	m_hapticPos(1) = p.x * 0.0001 - 0.080;
	m_hapticPos(2) = -p.y * 0.0001 + 0.080;

	m_velocity += 100.0 * timeStep * ((m_hapticPos - prev)/timeStep - m_velocity);

#else
	chai3d::cVector3d prev = m_hapticPos;

	device->getPosition(m_hapticPos);
	
	m_velocity += 100.0 * timeStep * ((m_hapticPos - prev)/timeStep - m_velocity);
#endif

}
	
void Racket::setPosition(const btVector3& position)
{
	m_hapticPos = Util::Vec(position);
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
