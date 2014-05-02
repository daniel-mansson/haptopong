#pragma once

#include "Scene.h"
#include "CollisionResponse.h"
#include "Ball.h"
#include "Racket.h"

class TestHapticCollisionScene : public Scene
{
public:
	TestHapticCollisionScene(Application& app);
	~TestHapticCollisionScene(void);

	void enter(ScenePtr from);
	void exit(ScenePtr to);

	void render(const double& timeStep);
	void updateLogic(const double& timeStep);
	void updateHaptics(const double& timeStep);

	void onKeyDown(unsigned char key, int x, int y);

	static ScenePtr create(Application& app) { return ScenePtr(new TestHapticCollisionScene(app)); }	

private:
	
	chai3d::cGenericHapticDevicePtr m_hapticDevice;
	chai3d::cWorldPtr m_world;
	chai3d::cCamera* m_camera;
	chai3d::cFontPtr m_font;
	std::vector<chai3d::cLabel*> m_labels;
	double m_maxLabelWidth;
	
	Ball m_ball;
	Racket m_racket;
	CollisionResponsePtr m_collision;
	bool m_force;
};

