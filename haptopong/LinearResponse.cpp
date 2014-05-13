#include "pch.h"
#include "LinearResponse.h"

#include "Ball.h"

#include "Racket.h"

using namespace chai3d;

LinearResponse::LinearResponse(const Racket& racket, const Ball& ball) :
	m_racket(racket),
	m_ball(ball)
{
	m_time = 0;
	m_fadeDuration = 0.01;

	m_duration = m_racket.getProperties().getElasticity() + m_ball.getProperties().getElasticity();	
	m_normal = m_racket.getNormal();
	
	//This is not correct
	m_tangent = Util::Vec(m_ball.getAngularVelocity());
	double angle = m_tangent.length();
	if(angle >= C_SMALL)
		m_tangent /= angle;


	m_magnitude = -10.0f * cDot(m_normal, Util::Vec(m_ball.getVelocity()) - m_racket.getVelocity());
	m_magnitudeTangent = 0.0f * angle;
}


LinearResponse::~LinearResponse(void)
{

}

void LinearResponse::updateHaptics(const double& timeStep, chai3d::cVector3d& force)
{
	m_time += timeStep;
	
	if(m_time >= m_duration)
	{
		force.zero();
		return;
	}

	double fade;
	if(m_time < m_fadeDuration)
		fade = m_time / m_fadeDuration;
	else if(m_time > m_duration - m_fadeDuration)
		fade = (m_duration - m_time) / m_fadeDuration;
	else 
		fade = 1.0;

	force = fade * (m_magnitude * m_normal + m_magnitudeTangent * m_tangent);
}

bool LinearResponse::isDone() const
{
	return m_time >= m_duration;
}