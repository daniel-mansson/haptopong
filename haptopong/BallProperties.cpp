#include "pch.h"
#include "BallProperties.h"

BallProperties::BallProperties(void) :
	m_friction(0.1),
	m_elasticity(0.02),
	m_restitution(0.9),
	m_weight(0.1)
{
}


BallProperties::~BallProperties(void)
{
}
