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
	m_fadeDuration = 0.02;

	m_duration = m_racket.getProperties().getElasticity() + m_ball.getProperties().getElasticity();	
	m_normal = m_racket.getNormal();
	
	m_tangent(0) = 0.0;
	m_tangent(1) = (double)m_ball.getAngularVelocity().z();
	m_tangent(2) = (double)m_ball.getAngularVelocity().y();

	//std::cout<<m_racket.getVelocity() * m_racket.getMoveAreaScale()<<"\t"<< Util::Vec(m_ball.getVelocity())<<"\n"; 
	m_magnitude = 6.5 * cDot(m_normal, Util::Vec(m_ball.getVelocity()) - 1.f*m_racket.getVelocity() * m_racket.getMoveAreaScale());
	m_magnitudeTangent = cAbs(m_magnitude * 0.0003);

	//std::cout<<"Mag: "<<m_tangent * m_magnitudeTangent<<"\tn "<<m_magnitude<<"\n";
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