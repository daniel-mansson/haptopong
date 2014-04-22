#pragma once
#include "Scene.h"

class TestScene : public Scene
{
public:
	TestScene(Application& app);
	~TestScene(void);

	void enter(ScenePtr from);
	void exit(ScenePtr to);

	void render(const double& timeStep);
	void updateLogic(const double& timeStep);
	void updateHaptics(const double& timeStep);

	void onKeyDown(unsigned char key, int x, int y);

	static ScenePtr create(Application& app) { return ScenePtr(new TestScene(app)); }

private:
	chai3d::cWorld* m_world;
	chai3d::cCamera* m_camera;
	chai3d::cDirectionalLight* m_light;
	chai3d::cShapeSphere* m_cursor;
	chai3d::cLabel* m_label;
};

