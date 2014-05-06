#include "pch.h"
#include "Ball.h"
#include "BallProperties.h"

using namespace chai3d;

Ball::Ball(chai3d::cShapeSphere* shape, btRigidBody* body) : 
	m_bernoulli(0, 0, 0),
	m_resistance(0, 0, 0),
	m_velocity(0, 0, 0),
	m_angularVelocity(0, 0, 0),
	m_shape(shape),
	m_body(body)
{
	//Regural movement(0.15[sec] is the updating time that is needed for integration)
	//m_acceleration += m_force
	//m_velocity += m_acceleration*0.15;
	//m_position += m_velocity*0.15;

	//Bernoulli
	
	//Bernoulli=2*(r*ƒÖ)^2*ƒÏ/2
	/*
	double radius = m_properties.getRadius();
	double weight = m_properties.getWeight();
    double volume = m_properties.getVolume();

	m_angularVelocity.normalize();

	
	double a = 2*radius*radius*m_angularVelocity*m_angularVelocity*weight/(volume*2);
	//m_acceleration += m_bernoulli/m_properties.getVolume();

	//gravity(it should only affec to Z)
	double gravity = m_properties.getGravity();
	//double acceleration -= gravity;

	//fluid resistance(0.4 is coefficient of resistance)
	double area = m_properties.getArea();
	double vel = m_velocity.length();
	double b = (weight/volume)*vel*vel*area*0.4/2;*/
	//m_acceleration -= m_resistance;
	
	if(m_body != nullptr)
		m_motionState = m_body->getMotionState();
}


Ball::~Ball(void)
{
}

void Ball::render(float timeStep)
{
	m_motionState->getWorldTransform(m_transform);

	m_shape->setLocalPos(Util::Vec(m_transform.getOrigin()));
	m_shape->setLocalRot(cMatrix3d(Util::Vec(m_transform.getRotation().getAxis()), m_transform.getRotation().getAngle()));

}

void Ball::updateLogic(float timeStep)
{
	btVector3 angVel = m_body->getAngularVelocity();
	btVector3 vel = m_body->getLinearVelocity();
	
	float angVelLen = angVel.length();
	angVel.safeNormalize();
	vel.safeNormalize();

	btVector3 cross = angVel.cross(vel);
	m_body->applyCentralForce(0.01f * angVelLen * cross);

}

void Ball::updateHaptics(float timeStep)
{

}

void Ball::onCollision(const btCollisionResult& collision)
{

}

void Ball::setPosition(const btVector3& position)
{
	m_body->translate(position - m_body->getCenterOfMassPosition());
}

void Ball::setVelocity(const btVector3& velocity)
{
	m_body->setLinearVelocity(velocity);
}

void Ball::setAngularVelocity(const btVector3& angularVelocity)
{
	m_body->setAngularVelocity(angularVelocity);
}

void Ball::setAngularVelocity(const btVector3& axis, float angle)
{
	m_body->setAngularVelocity(angle * axis);
}

const btVector3& Ball::getPosition() const
{
	return m_body->getCenterOfMassPosition();
}

const btVector3& Ball::getVelocity() const
{
	return m_body->getLinearVelocity();
}

const btVector3& Ball::getAngularVelocity() const
{
	return m_body->getAngularVelocity();
}

void Ball::stop()
{
	m_body->setLinearVelocity(btVector3(0,0,0));
	m_body->setAngularVelocity(btVector3(0,0,0));
}