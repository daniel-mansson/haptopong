#pragma once
#include "Scene.h"

#include "Table.h"
#include "Ball.h"
#include "Racket.h"

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
	btRigidBody* m_groundBody;
	chai3d::cShapeBox* m_ground;
	
	TablePtr m_table;
	BallPtr m_ball;
    
	void createCamera();
    void createLight();
	void createTable();
	void createBall();
};

