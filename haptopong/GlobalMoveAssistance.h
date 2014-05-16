#pragma once
#include "AimAssistance.h"
class GlobalMoveAssistance :
	public AimAssistance
{
public:
	GlobalMoveAssistance(BallPtr ball, RacketPtr playerRacket, chai3d::cCamera* camera);
	virtual ~GlobalMoveAssistance(void);
	
	virtual void render(const double& timeStep);
	virtual void updateLogic(const double& timeStep);
	virtual void updateHaptics(const double& timeStep, chai3d::cVector3d& force);

	virtual void applyImpulseFromRacket(btManifoldPoint& point);

private:
};

