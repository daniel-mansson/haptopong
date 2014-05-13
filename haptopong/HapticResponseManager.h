#pragma once

#include "CollisionResponse.h"

class HapticResponseManager
{
public:
	HapticResponseManager(void);
	~HapticResponseManager(void);

	void setCurrent(CollisionResponsePtr current) { m_current = current; }

	void updateHaptics(const double& timeStep, chai3d::cVector3d& force);

private:
	CollisionResponsePtr m_current;
};

typedef std::shared_ptr<HapticResponseManager> HapticResponseManagerPtr;