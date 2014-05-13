#pragma once

#include "GameObject.h"

class Net : public GameObject
{
public:
	Net(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape);
	~Net(void);
	
	virtual GameObjectType getType() const { return NET; };
    
	virtual void render(float timeStep);
	virtual void updateLogic(float timeStep);
	virtual void updateHaptics(chai3d::cGenericHapticDevicePtr device, float timeStep);
    
	virtual void onCollision(const btCollisionResult& collision);
    
    btRigidBody* getBody() const;
    chai3d::cMultiMesh* getShape() const;
    
private:
    
	chai3d::cMultiMesh* m_shape;
	btRigidBodyPtr m_body;
};

typedef std::shared_ptr<Net> NetPtr;