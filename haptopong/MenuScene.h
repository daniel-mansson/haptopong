#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene(Application& app);
	~MenuScene(void);

	void enter(ScenePtr from);
	void exit(ScenePtr to);

	void render(double timeStep);
	void updateLogic(double timeStep);
	void updateHaptics(double timeStep);

	void onKeyDown(unsigned char key, int x, int y);

	static ScenePtr create(Application& app) { return ScenePtr(new MenuScene(app)); }

private:
	
	chai3d::cWorldPtr m_world;
	chai3d::cCamera* m_camera;

	std::vector<chai3d::cLabel*> m_menu;
	chai3d::cLabel* m_title;
	chai3d::cLabel* m_subtitle;
	std::vector<chai3d::cFontPtr> m_fonts;

	double m_maxMenuWidth;
};

