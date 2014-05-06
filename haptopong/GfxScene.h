#pragma once
#include "Scene.h"

class GfxScene : public Scene
{
public:
	GfxScene(Application& app);
	~GfxScene(void);
    
	void enter(ScenePtr from);
	void exit(ScenePtr to);
    
	void render(const double& timeStep);
	void updateLogic(const double& timeStep);
	void updateHaptics(const double& timeStep);
    
	void onKeyDown(unsigned char key, int x, int y);
    
	static ScenePtr create(Application& app) { return ScenePtr(new GfxScene(app)); }
    
private:
	chai3d::cWorld* m_world;
	chai3d::cCamera* m_camera;
	chai3d::cDirectionalLight* m_light;
	chai3d::cLabel* m_label;
	chai3d::cMultiMesh* m_table;
	chai3d::cMultiMesh* m_net;
	chai3d::cMultiMesh* m_playerRacket;
	chai3d::cMultiMesh* m_opponentRacket;
    chai3d::cHapticDeviceHandler* m_handler;
    chai3d::cGenericHapticDevicePtr m_hapticDevice;
    chai3d::cToolCursor* m_tool;
    double m_playerTransparency;
};

