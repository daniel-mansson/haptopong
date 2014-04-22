#pragma once
#include "CollisionResponse.h"

class Racket;
class Ball;

class LinearResponse : public CollisionResponse
{
public:
	LinearResponse(const Racket& racket, const Ball& ball);
	~LinearResponse(void);

	void updateHaptics(const double& timeStep, chai3d::cVector3d& force);
	bool isDone() const;

private:
	const Racket& m_racket;
	const Ball& m_ball;

	double m_time;
	double m_duration;
	double m_magnitude;
	double m_magnitudeTangent;
	double m_fadeDuration;
	chai3d::cVector3d m_normal;
	chai3d::cVector3d m_tangent;
};

