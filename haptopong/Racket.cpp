#include "pch.h"
#include "Racket.h"
#include "BallProperties.h"


Racket::Racket(void) :
	m_normal(1, 0, 0),
	m_velocity(0, 0, 0),
	m_force(0, 0, 0)
{
	//0.000034 represents volume
	m_force = m_velocity*m_velocity*0.000034/2;
	}


Racket::~Racket(void)
{
}
