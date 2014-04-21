#pragma once

#include "Scene.h"

class Application
{
public:
	Application(void);
	~Application(void);

	bool initialize(const std::string& title, int* argc, char** argv);	
	void run();

	//Callbacks
	void resizeWindow(int w, int h);
	void keySelect(unsigned char key, int x, int y);
	void updateGraphics(void);
	void graphicsTimer(int data);
	void updateHaptics();

	//Scene stack
	void pushScene(ScenePtr scene);
	ScenePtr popScene();
	void clearScenes();

	//Getters/setters
	int getWindowWidth() const { return m_windowWidth; }
	int getWindowHeight() const { return m_windowHeight; }
	const double& getSmoothRenderTimeStep() const { return m_smoothRenderTimeStep; }
	const double& getSmoothLogicTimeStep() const { return m_smoothLogicTimeStep; }
	const double& getSmoothHapticTimeStep() const { return m_smoothHapticTimeStep; }
	double getHapticHz() const { return m_frequencyCounter.getFrequency(); }

private:
	chai3d::cStereoMode m_stereoMode;
	bool m_fullscreen;
	volatile bool m_simulationRunning;
	volatile bool m_simulationFinished;
	int m_windowWidth;
	int m_windowHeight;
	chai3d::cGenericHapticDevicePtr m_hapticDevice;
	std::vector<ScenePtr> m_sceneStack;
	chai3d::cPrecisionClock m_timer;
    chai3d::cFrequencyCounter m_frequencyCounter;
	double m_smoothRenderTimeStep;
	double m_smoothLogicTimeStep;
	double m_smoothHapticTimeStep;
	double m_timeStepSmoothFactor;
	
	void close();
};

