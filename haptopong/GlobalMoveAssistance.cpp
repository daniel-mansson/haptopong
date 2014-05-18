#include "pch.h"
#include "GlobalMoveAssistance.h"

using namespace chai3d;

GlobalMoveAssistance::GlobalMoveAssistance(BallPtr ball, RacketPtr playerRacket, chai3d::cCamera* camera) :
	AimAssistance(ball, playerRacket, camera),
	m_guideForce(5.0),
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
	
	double v = bpos(0) - 0.5;
	double factor = cClamp01(v) * cClamp01(3 - bpos(0));
	
	toBall(0) = 0.0;
	double dist = toBall.length();
	toBall /= dist;
	
	dist = cClamp01(dist / 1.0);
	dist = sqrt(dist);
	dist *= 1;

	double f = cClamp(factor * m_guideForce * dist, -5.0, 5.0);

	force += f * toBall;


}

void GlobalMoveAssistance::applyImpulseFromRacket(btManifoldPoint& point)
{
	AimAssistance::applyImpulseFromRacket(point);

	//changeVel(btVector3(-1.0, 0, 0));
}
