#pragma once

#include "GameObject.h"

class Table : public GameObject
{
public:
	Table(chai3d::cMultiMesh* mesh, btRigidBody* body);
	~Table(void);
	
	virtual GameObjectType getType() const { return TABLE; };

	virtual void render(float timeStep);
	virtual void updateLogic(float timeStep);
	virtual void updateHaptics(float timeStep);

	virtual void onCollision(const btCollisionResult& collision);
    
    std::string s;

private:

	chai3d::cMultiMesh* m_mesh;
	btRigidBody* m_body;
};

typedef std::shared_ptr<Table> TablePtr;