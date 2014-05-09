#pragma once

#include "GameObject.h"

class Table : public GameObject
{
public:
	Table(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape);
	~Table(void);
	
	virtual GameObjectType getType() const { return TABLE; };

	virtual void render(float timeStep);
	virtual void updateLogic(float timeStep);
	virtual void updateHaptics(float timeStep);

	virtual void onCollision(const btCollisionResult& collision);
    
    btRigidBody* getBody() const;
    chai3d::cMultiMesh* getShape() const;

private:

	chai3d::cMultiMesh* m_shape;
	btRigidBodyPtr m_body;
};

typedef std::shared_ptr<Table> TablePtr;