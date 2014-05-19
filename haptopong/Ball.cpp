#include "pch.h"
#include "Ball.h"
#include "BallProperties.h"

using namespace chai3d;

Ball::Ball(cGenericObject* shape, btCollisionShape* collisionShape, const cTexture2dPtr &playTexture, const cTexture2dPtr &inactiveTexture,
           const BallProperties &properties, const btTransform &startTransform) :
	m_bernoulli(0, 0, 0),
	m_resistance(0, 0, 0),
	m_velocity(0, 0, 0),
	m_angularVelocity(0, 0, 0),
	m_shape(shape),
	m_properties(properties),
	m_isActive(true)
{   
	//btScalar mass = m_properties.getWeight();
	btScalar mass(1.f);
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		collisionShape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, collisionShape, localInertia);

	m_body = std::make_shared<btRigidBody>(rbInfo);
	m_body->setRestitution((btScalar)m_properties.getRestitution());
	m_body->setDamping((float)m_properties.getLinDamping(), (float)m_properties.getAngDamping());
	m_body->setUserPointer(this);
	m_body->setActivationState(DISABLE_DEACTIVATION);

	setVelocity(btVector3(Util::RandRange(-0.2f, 0.2f), Util::RandRange(-0.2f, 0.2f), Util::RandRange(-0.2f, 0.2f)));
	//m_body->setCcdMotionThreshold(m_properties.getRadius()*0.9);
	//m_body->setCcdSweptSphereRadius(m_properties.getRadius()*0.9);


	//m_shadowShape->scale(0.9);


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
	
	m_playMat = *m_shape->m_material;
	m_inactiveMat = *m_shape->m_material;
	//m_inactiveMat.m_diffuse.set(0.5f, 0.0f, 0.0f);
	//m_inactiveMat.m_ambient.set(0.3f, 0.1f, 0.1f);

    m_playTex = playTexture;
    m_inactiveTex = inactiveTexture;
    m_shape->setTexture(std::make_shared<cTexture2d>(*m_playTex.get())); // need to be copy. image changes need to be confined
	m_shape->setUseTexture(true, true);
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

	//Not really correct, some kind of dot product is missing
	m_body->applyCentralForce(0.015f * angVelLen * angVel.cross(vel));
}

void Ball::onRoundStart()
{
	//m_shape->setMaterial(m_playMat);
    //m_shape->setTexture(m_playTex);
    m_shape->m_texture->m_image = m_playTex->m_image;
    m_shape->m_texture->markForUpdate();
}

void Ball::onRoundEnd()
{
	//m_shape->setMaterial(m_inactiveMat);
    //m_shape->setTexture(m_inactiveTex);
    m_shape->m_texture->m_image = m_inactiveTex->m_image;
    m_shape->m_texture->markForUpdate();
}

void Ball::updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep)
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

btRigidBody* Ball::getBody() const {
	return m_body.get();
}

chai3d::cGenericObject* Ball::getShape() const {
	return m_shape;
}

void Ball::stop()
{
	m_body->setLinearVelocity(btVector3(0,0,0));
	m_body->setAngularVelocity(btVector3(0,0,0));
}
