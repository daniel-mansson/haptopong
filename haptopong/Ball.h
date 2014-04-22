#pragma once

#include "BallProperties.h"

class Ball
{
public:
	Ball(void);
	~Ball(void);

	void setProperties(const BallProperties& properties) { m_properties = properties; }
		
	//Setters for testing (should be updated by the physics engine later)
	void setVelocity(const chai3d::cVector3d& velocity) { m_velocity = velocity; }
	void setAngularVelocity(const chai3d::cQuaternion& angularVelocity) { m_angularVelocity = angularVelocity; }

	const BallProperties& getProperties() const { return m_properties; }
	const chai3d::cVector3d& getVelocity() const { return m_velocity; }
	const chai3d::cQuaternion& getAngularVelocity() const { return m_angularVelocity; }

private:
	chai3d::cVector3d m_velocity;
	chai3d::cQuaternion m_angularVelocity;
	BallProperties m_properties;
};

