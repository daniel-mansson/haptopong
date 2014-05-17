#include "pch.h"
#include "Racket.h"
#include "BallProperties.h"

using namespace chai3d;

Racket::Racket(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape, const RacketProperties &properties, const btTransform &startTransform) :
	m_normal(-1, 0, 0),
	m_velocity(0, 0, 0),
	m_force(0, 0, 0),
	m_shape(shape),
	m_hapticPos(0,0,0),
	m_moveAreaScale(10.0),
	m_playerId(NO_PLAYER),
	m_currentScale(1.0),
	m_isOpponent(false)
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


	m_body->getMotionState()->getWorldTransform(m_transform);
	m_origin = m_transform.getOrigin();
	m_startPos = Util::Vec(m_origin);

	m_prevHapticPos = m_hapticPos;
	m_time = 0;
	m_maxTime = 100;
	
	m_material = *m_shape->m_material;
	m_flashMaterial = *m_shape->m_material;
	m_flash = 0.0;
	m_flashFactor = 1.0 - m_material.m_ambient.getR();
}


Racket::~Racket(void)
{
}

void Racket::flash()
{
	m_flash = 1.0;
	
}

void Racket::setSize(double size)
{
	m_body->getCollisionShape()->setLocalScaling(btVector3(1.0f, (float)size, (float)size));
	double s = size / m_currentScale;	
	m_shape->scaleXYZ(1.0f, (float)s, (float)s);
	m_currentScale = size;
}

void Racket::render(float timeStep)
{
	m_flash -= 3.0 * timeStep * m_flash;
	
	float f = (float)(m_flash * m_flashFactor);
	m_flashMaterial.m_ambient.set(
		m_material.m_ambient.getR() + f,
		m_material.m_ambient.getG() + f,
		m_material.m_ambient.getB() + f);


	m_shape->setMaterial(m_flashMaterial, true);
	m_shape->setUseTransparency(true);
	m_shape->setTransparencyLevel(0.6f);

	btTransform transform;

	btMotionState* pState = m_body->getMotionState();
	pState->getWorldTransform(transform);

	m_shape->setLocalPos(Util::Vec(transform.getOrigin()));
	m_shape->setLocalRot(chai3d::cMatrix3d(Util::Vec(transform.getRotation().getAxis()), transform.getRotation().getAngle()));
}

void Racket::updateLogic(float timeStep)
{
	if(m_isOpponent)
	{
		double t = m_time / m_maxTime;

		chai3d::cVector3d offset = m_prevHapticPos + (m_hapticPos - m_prevHapticPos) * t;
		offset *= m_moveAreaScale;

		m_transform.setOrigin(Util::Vec(offset) + m_origin);
		m_body->getMotionState()->setWorldTransform(m_transform);

		m_time += timeStep;
	}
	else
	{
		chai3d::cVector3d offset = m_hapticPos;
		offset *= m_moveAreaScale;

		m_transform.setOrigin(Util::Vec(offset) + m_origin);
		m_body->getMotionState()->setWorldTransform(m_transform);
	}
}

//#define USE_MOUSE

void Racket::updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep)
{
#ifdef USE_MOUSE


	POINT p;
	GetCursorPos(&p);

	m_hapticPos(0) = 0.0;
	m_hapticPos(1) = p.x * 0.0001 - 0.080;
	m_hapticPos(2) = -p.y * 0.0001 + 0.080;

	chai3d::cVector3d prev = m_hapticPos;
	prev(0) += 0.0018;

#else
	chai3d::cVector3d prev = m_hapticPos;

	device->getPosition(m_hapticPos);
#endif

	m_hapticVel = m_hapticPos - prev;
	m_velocity += 50.0 * timeStep * (m_hapticVel / timeStep - m_velocity);

}

void Racket::setPosition(const btVector3& position)
{
	m_hapticPos = Util::Vec(position);
}

void Racket::setPosition(const btVector3& position, double timeSinceLastUpdate)
{
	m_maxTime = timeSinceLastUpdate;
	m_prevHapticPos = m_hapticPos;
	m_hapticPos = Util::Vec(position);
	m_time = 0.0;
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
