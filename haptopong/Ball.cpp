#include "pch.h"
#include "Ball.h"
#include "BallProperties.h"

using namespace chai3d;

Ball::Ball(void) : 
	//m_position(0, 0, 0),
	m_velocity(0, 0, 0),
	//m_acceleration(0, 0, 0),
	m_angularVelocity(0, 0, 0),
	m_bernoulli(0, 0, 0),
	m_resistance(0, 0, 0)
	
	
{
	//Regural movement(0.15[sec] is the updating time that is needed for integration)
	//m_acceleration += m_force
	//m_velocity += m_acceleration*0.15;
	//m_position += m_velocity*0.15;

	//Bernoulli
	
	//Bernoulli=2*(r*É÷)^2*Éœ/2

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
	double b = (weight/volume)*vel*vel*area*0.4/2;
	//m_acceleration -= m_resistance;
	
}


Ball::~Ball(void)
{
}
