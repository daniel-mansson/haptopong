#include "pch.h"
#include "RacketProperties.h"


RacketProperties::RacketProperties(void):
	m_friction(0.1),
	m_elasticity(0.1),
	m_restitution(0.9),
	m_weight(0.7)
{
}


RacketProperties::~RacketProperties(void)
{
}
