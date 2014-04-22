#pragma once

#include "BallProperties.h"

class Ball
{
public:
	Ball(void);
	~Ball(void);

	void setProperties(const BallProperties& properties) { m_properties = properties; }

	const BallProperties& getProperties() const { return m_properties; }
	const chai3d::cVector3d& getVelocity() const { return m_velocity; }
	const chai3d::cQuaternion& getAngularVelocity() const { return m_angularVelocity; }

private:
	chai3d::cVector3d m_velocity;
	chai3d::cQuaternion m_angularVelocity;
	BallProperties m_properties;
};

