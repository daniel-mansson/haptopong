#include "pch.h"
#include "HapticResponseManager.h"

HapticResponseManager::HapticResponseManager(void) 
{
}


HapticResponseManager::~HapticResponseManager(void)
{
}

void HapticResponseManager::updateHaptics(const double& timeStep, chai3d::cVector3d& force)
{
	CollisionResponsePtr response = m_current;

	if(response != nullptr)
	{
		if(response->isDone())
			m_current = nullptr;
		else
			response->updateHaptics(timeStep, force);
	}
}