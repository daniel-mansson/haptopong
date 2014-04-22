#pragma once

#include "Scene.h"

// Initializes and manages the haptic and graphical devices.
//
// It is also a basic scene manager. The scenes are organized as
// a stack container, where only the top scene is active. The
// scenes can be pushed and popped to manage transitions between
// different scenes. The application will exit when the stack is
// empty.
class Application
{
public:
	Application(void);
	~Application(void);

	// Called to initialize all sub systems. Returns true if all succeeded.
	bool initialize(const std::string& title, int* argc, char** argv);	
	// Start the application. Will block until the application is closed.
	void run();

	// Push a scene to the top of the stack
	void pushScene(ScenePtr scene);
	// Pop the top scene from the stack and return it.
	ScenePtr popScene();
	// Pop all scenes from the stack.
	void clearScenes();

	int getWindowWidth() const { return m_windowWidth; }
	int getWindowHeight() const { return m_windowHeight; }
	double getHapticHz() const { return m_frequencyCounter.getFrequency(); }
	const double& getSmoothRenderTimeStep() const { return m_smoothRenderTimeStep; }
	const double& getSmoothLogicTimeStep() const { return m_smoothLogicTimeStep; }
	const double& getSmoothHapticTimeStep() const { return m_smoothHapticTimeStep; }
	chai3d::cGenericHapticDevicePtr getHapticDevice() const { return m_hapticDevice; }

private:
	//Display info
	chai3d::cStereoMode m_stereoMode;
	bool m_fullscreen;
	int m_windowWidth;
	int m_windowHeight;

	// Simulation state
	volatile bool m_simulationRunning;
	volatile bool m_simulationFinished;

	// Haptic device
	chai3d::cGenericHapticDevicePtr m_hapticDevice;

	// Scene stack
	std::vector<ScenePtr> m_sceneStack;
	ScenePtr m_currentScene;

	//Update rate counters
    chai3d::cFrequencyCounter m_frequencyCounter;
	double m_smoothRenderTimeStep;
	double m_smoothLogicTimeStep;
	double m_smoothHapticTimeStep;
	double m_timeStepSmoothFactor;
	chai3d::cPrecisionClock m_timer;
	
	// Close the application
	void close();

private:
	// Callbacks
	void resizeWindow(int w, int h);
	void keySelect(unsigned char key, int x, int y);
	void updateGraphics(void);
	void graphicsTimer(int data);
	void updateHaptics();
	
	static void callbackResizeWindow(int w, int h);
	static void callbackKeySelect(unsigned char key, int x, int y);
	static void callbackUpdateGraphics(void);
	static void callbackGraphicsTimer(int data);
	static void updateHapticsThread();
};

