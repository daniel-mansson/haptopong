#pragma once

#include "GameObject.h"

class Outside : public GameObject
{
public:
	Outside(btCollisionShape* collisionShape);
	~Outside(void);
	
	virtual GameObjectType getType() const { return OUTSIDE; };

	virtual void render(float timeStep);
	virtual void updateLogic(float timeStep);
	virtual void updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep);

	virtual void onCollision(const btCollisionResult& collision);
    
	btRigidBodyPtr getBody() { return m_body; }

private:

	btRigidBodyPtr m_body;
};

typedef std::shared_ptr<Outside> OutsidePtr;