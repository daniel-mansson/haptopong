#pragma once
#include "Scene.h"

class NetworkScene : public Scene
{
public:
	NetworkScene(Application& app);
	~NetworkScene(void);

	void enter(ScenePtr from);
	void exit(ScenePtr to);

	void render(const double& timeStep);
	void updateLogic(const double& timeStep);
	void updateHaptics(const double& timeStep);

	void onKeyDown(unsigned char key, int x, int y);

	static ScenePtr create(Application& app) { return ScenePtr(new NetworkScene(app)); }

private:
	
	chai3d::cWorldPtr m_world;
	chai3d::cCamera* m_camera;
	
	chai3d::cFontPtr m_font;
	std::vector<chai3d::cLabel*> m_labels;
	int m_state;

	ENetAddress m_address;
	ENetHost* m_server;

	ENetHost* m_client;
	ENetPeer* m_peer;

	void print(const std::string& text);
	void clear();
};

