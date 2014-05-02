#pragma once
#include "Scene.h"

class PongScene : public Scene
{
public:
	PongScene(Application& app);
	~PongScene(void);

	void enter(ScenePtr from);
	void exit(ScenePtr to);

	void render(const double& timeStep);
	void updateLogic(const double& timeStep);
	void updateHaptics(const double& timeStep);

	void onKeyDown(unsigned char key, int x, int y);

	static ScenePtr create(Application& app) { return ScenePtr(new PongScene(app)); }

private:
	 
	chai3d::cWorldPtr m_world;
	chai3d::cCamera* m_camera;
	
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	
	btCollisionShape* m_groundShape;
	btCollisionShape* m_sphereShape;
	btRigidBody* m_groundBody;
	btRigidBody* m_sphereBody;
};

