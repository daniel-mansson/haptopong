#pragma once

class Application;

class Scene
{
public:
	Scene(Application& app);
	virtual ~Scene(void);
	
	virtual void enter(std::shared_ptr<Scene> from) = 0;
	virtual void exit(std::shared_ptr<Scene> to) = 0;

	virtual void render(double timeStep) = 0;
	virtual void updateLogic(double timeStep) = 0;
	virtual void updateHaptics(double timeStep) = 0;

	virtual void onKeyDown(unsigned char key, int x, int y) = 0;

protected:
	Application& m_app;
};

typedef std::shared_ptr<Scene> ScenePtr;