#pragma once

#include "Ball.h"
#include "Racket.h"

class AimAssistance
{
public:
	AimAssistance(BallPtr ball, RacketPtr playerRacket, chai3d::cCamera* camera);
	virtual ~AimAssistance(void);
	
	virtual void render(const double& timeStep) { };
	virtual void updateLogic(const double& timeStep) { };
	virtual void updateHaptics(const double& timeStep, chai3d::cVector3d& force) {  };

	virtual void applyImpulseFromRacket(btManifoldPoint& point);

private:

	BallPtr m_ball;
	RacketPtr m_racket;
	chai3d::cCamera* m_camera;
};

typedef std::shared_ptr<AimAssistance> AimAssistancePtr;
