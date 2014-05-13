#pragma once

#include "GameObjectType.h"

class GameObject
{
public:
	GameObject(void);
	virtual ~GameObject(void);

	virtual GameObjectType getType() const = 0;
	
	virtual void render(float timeStep) = 0;
	virtual void updateLogic(float timeStep) = 0;
	virtual void updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep) = 0;

	virtual void onCollision(const btCollisionResult& collision) = 0;
};

