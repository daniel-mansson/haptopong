#include "pch.h"
#include "AimAssistance.h"

using namespace chai3d;

AimAssistance::AimAssistance(BallPtr ball, RacketPtr playerRacket, chai3d::cCamera* camera) :
	m_ball(ball),
	m_racket(playerRacket),
	m_camera(camera)
{
}


AimAssistance::~AimAssistance(void)
{
}

void AimAssistance::applyImpulseFromRacket(btManifoldPoint& point)
{
	btVector3 bvel = m_ball->getVelocity();
	btVector3 rvel = Util::Vec(m_racket->getVelocity() * m_racket->getMoveAreaScale());

	float xvel = rvel[0] - bvel[0];
	float yvel = rvel[1] - bvel[1];
	float zvel = rvel[2] - bvel[2];

	//TODO: real forces
	xvel *= (1.0f + point.m_combinedRestitution * 0.6f);	
	bvel[0] += xvel;
	bvel[1] += yvel * 0.7f;
	bvel[2] += zvel * 0.7f + fabsf(xvel * 0.1f);

	float len = bvel.length();
	if(len > 5.5f)
		bvel *= 5.5f / len;

	m_ball->setVelocity(bvel);

	//std::cout<<"HIT! "<< Util::Vec(bvel)<<"\n";

	btVector3 angVel = m_ball->getAngularVelocity();
	angVel[1] += -rvel[2] * 240.0f;
	angVel[2] += rvel[1] * 240.0f;
	//std::cout<< "-Y "<<-rvel[2] * 200.0f<<"\t|Z "<< rvel[1] * 200.0f<<"\n";
	m_ball->setAngularVelocity(angVel);

}

void AimAssistance::changeVel(const btVector3& target)
{
	/*btVector3 dist = target - m_ball->getPosition();

	float v0 = m_ball->getVelocity().x();
	float a = 0.0;	
	float v0_a = v0 / a;

	float timeX = dist.x() / v0;

	float zPos = m_ball->getVelocity().z() * timeX + -10.0 * timeX * timeX * 0.5f;

	btVector3 vel = m_ball->getVelocity();
	vel[2] -= zPos + 1.0;
	m_ball->setVelocity(vel);

	std::cout 
		<<"Time: " << timeX << "\t"
		<<"v0: " << v0 << "\t"
		<<"distx: " << dist.x() << "\t"
		<<"zpos: " << zPos << "\t"
		<<std::endl;*/

	//std::cout<<"Angvel: "<<Util::Vec(m_ball->getAngularVelocity())<<"\tVel: "<<Util::Vec(m_ball->getVelocity())<<"\tAdjvel: "<<Util::Vec(adjustedVelocity(m_ball->getVelocity(), m_ball->getAngularVelocity(), 0.03))<<"\n";
}

btVector3 AimAssistance::adjustedVelocity(const btVector3& velocity, const btVector3& angularVel, float factor)
{
	btVector3 adjVel;

	factor *= -(float)C_PI / 180.0f;

	btMatrix3x3 m;
	m.setEulerZYX(angularVel.x() * factor, angularVel.y() * factor, angularVel.z() * factor);

	adjVel = velocity * m;

	return adjVel;
}