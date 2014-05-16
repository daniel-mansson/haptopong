#include "pch.h"
#include "GlobalMoveAssistance.h"

using namespace chai3d;

GlobalMoveAssistance::GlobalMoveAssistance(BallPtr ball, RacketPtr playerRacket, chai3d::cCamera* camera) :
	AimAssistance(ball, playerRacket, camera)
{
	playerRacket->setMoveAreaScale(18.0f);
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
	cVector3d bpos = Util::Vec(m_ball->getPosition());
	cVector3d rpos = m_racket->getHapticWorldPos();
	
	double factorX = cClamp01(bpos.x());
	double factorZ = cClamp01(2.0 + bpos.z());

	cVector3d vec = factorX * factorZ * 5.0 * (bpos - rpos);
	vec(0) = 0;
	
	double mag = 3.0;
	vec(1) = cClamp(vec(1), -mag, mag);
	vec(2) = cClamp(vec(2), -mag, mag);

	force += vec;

}

void GlobalMoveAssistance::applyImpulseFromRacket(btManifoldPoint& point)
{
	AimAssistance::applyImpulseFromRacket(point);

	changeVel(btVector3(-1.0, 0, 0));
}
