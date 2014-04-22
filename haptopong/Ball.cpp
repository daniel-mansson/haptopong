#include "pch.h"
#include "Ball.h"


Ball::Ball(void) : 
	m_velocity(0, 0, 0),
	m_angularVelocity(0, 0, 0, 1)
{
	
}


Ball::~Ball(void)
{
}
