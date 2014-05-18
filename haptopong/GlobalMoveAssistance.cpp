#include "pch.h"
#include "GlobalMoveAssistance.h"

using namespace chai3d;

GlobalMoveAssistance::GlobalMoveAssistance(BallPtr ball, RacketPtr playerRacket, chai3d::cCamera* camera) :
	AimAssistance(ball, playerRacket, camera),
	m_guideForce(10.0),
	m_useGuiding(false)
{
}


GlobalMoveAssistance::~GlobalMoveAssistance(void)
{
}
 
void GlobalMoveAssistance::render(const double& timeStep)
{
	btVector3 pos = m_racket->getPosition();
	pos *= m_racket->getMoveAreaScale();

	m_camera->set(
		cVector3d( 3.0 + pos.x() * 0.55, pos.y() * 0.35, 1.0),
		cVector3d( -1.0, 0.0, 0.0),
		cVector3d( 0.0, 0.0, 1.0));
}

void GlobalMoveAssistance::updateLogic(const double& timeStep)
{

}

void GlobalMoveAssistance::updateHaptics(const double& timeStep, chai3d::cVector3d& force)
{
	static int c = 0;

	if(!m_useGuiding)
		return;

	cVector3d bpos = Util::Vec(m_ball->getPosition());
	cVector3d rpos = m_racket->getHapticWorldPos();

	cVector3d toBall = bpos - rpos;
	
	double v = cAbs(toBall(0) * 0.5);
	double factor = cClamp01(1.0 - v);
	
	toBall(0) = 0.0;
	double dist = toBall.length();
	toBall /= dist;
	/*
	dist = cClamp01(dist / 3.0);
	dist *= cSqr(dist);
	dist *= 3;*/

	double f = cClamp(factor * m_guideForce * dist, -5.0, 5.0);

	force += f * toBall;


}

void GlobalMoveAssistance::applyImpulseFromRacket(btManifoldPoint& point)
{
	AimAssistance::applyImpulseFromRacket(point);

	//changeVel(btVector3(-1.0, 0, 0));
}
