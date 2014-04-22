#pragma once

class Application;

// Abstract base class for a general scene. 
// Manages scene data and handles events.
class Scene
{
public:
	Scene(Application& app);
	virtual ~Scene(void);
	
	// Called when the application enter this scene
	virtual void enter(std::shared_ptr<Scene> from) = 0;
	// Called when the application leaves this scene
	virtual void exit(std::shared_ptr<Scene> to) = 0;

	// Render the scene
	virtual void render(const double& timeStep) = 0;
	// Update the logic in the scene (Often with a time step similar to render)
	virtual void updateLogic(const double& timeStep) = 0;
	// Update the haptics rendering (Needs to be high performance and thread safe)
	virtual void updateHaptics(const double& timeStep) = 0;

	// Called when a keyboard button is pressed
	virtual void onKeyDown(unsigned char key, int x, int y) = 0;

protected:
	Application& m_app;
};

typedef std::shared_ptr<Scene> ScenePtr;