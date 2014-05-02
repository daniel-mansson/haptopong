#pragma once

#include "BallProperties.h"

class Ball
{
public:
	Ball(void);
	~Ball(void);

	void setProperties(const BallProperties& properties) { m_properties = properties; }
		
	//Setters for testing (should be updated by the physics engine later)
	void setPosition(const chai3d::cVector3d& position) { m_position = position; }
	void setVelocity(const chai3d::cVector3d& velocity) { m_velocity = velocity; }
	void setAcceleration(const chai3d::cVector3d& acceleration) { m_velocity = acceleration; }
	void setAngularVelocity(const chai3d::cVector3d& angularVelocity) { m_angularVelocity = angularVelocity; }
	void setAngularVelocity(const chai3d::cVector3d& axis, const double& angle) { m_angularVelocity = angle * axis; }
	void setBernulli(const chai3d::cVector3d& bernoulli) { m_bernoulli = bernoulli; }
	void setResistance(const chai3d::cVector3d& resistance) { m_resistance = resistance; }
	

	const BallProperties& getProperties() const { return m_properties; }
	const chai3d::cVector3d& getPosition() const { return m_position; }
	const chai3d::cVector3d& getVelocity() const { return m_velocity; }
	const chai3d::cVector3d& getAcceleration() const { return m_acceleration; }
	const chai3d::cVector3d& getAngularVelocity() const { return m_angularVelocity; }
	const chai3d::cVector3d& getBernoulli() const { return m_bernoulli; }
	const chai3d::cVector3d& getResistance() const { return m_resistance; }
	

private:
	chai3d::cVector3d m_position;
	chai3d::cVector3d m_velocity;
	chai3d::cVector3d m_acceleration;
	chai3d::cVector3d m_angularVelocity;
	chai3d::cVector3d m_bernoulli;
	chai3d::cVector3d m_resistance;
	BallProperties m_properties;

};

