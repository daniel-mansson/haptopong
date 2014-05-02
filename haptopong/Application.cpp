#include "pch.h"
#include "Application.h"

using namespace chai3d;

// Global variable to the first application object created.
// Used for the callback functions to get a reference to 
// the current instance.
Application* g_app = nullptr;

Application::Application(void) : 
	m_stereoMode(cStereoMode::C_STEREO_DISABLED),
	m_fullscreen(false),
	m_simulationRunning(false),
	m_simulationFinished(false),
	m_windowWidth(0),
	m_windowHeight(0),
	m_timeStepSmoothFactor(0.1),
	m_smoothHapticTimeStep(0.1),
	m_smoothLogicTimeStep(0.1),
	m_smoothRenderTimeStep(0.1)
{
}

Application::~Application(void)
{
	g_app = nullptr;
}

bool Application::initialize(const std::string& title, int* argc, char** argv)
{
	g_app = this;

	glutInit(argc, argv);

	int screenW = glutGet(GLUT_SCREEN_WIDTH);
	int screenH = glutGet(GLUT_SCREEN_HEIGHT);
	m_windowWidth = (int)(0.8 * screenH);
	m_windowHeight = (int)(0.5 * screenH);
	int windowPosY = (screenH - m_windowHeight) / 2;
	int windowPosX = windowPosY;

	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(m_windowWidth, m_windowHeight);

	if (m_stereoMode == C_STEREO_ACTIVE)
		glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STEREO);
	else
		glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

	// create display context and initialize GLEW library
	glutCreateWindow(argv[0]);
	glewInit();

	// setup GLUT options
	glutDisplayFunc(callbackUpdateGraphics);
	glutKeyboardFunc(callbackKeySelect);
	glutReshapeFunc(callbackResizeWindow);
	glutSetWindowTitle(title.c_str());
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

	// set fullscreen mode
	if (m_fullscreen)
		glutFullScreen();

	// create a haptic device handler
	cHapticDeviceHandler* handler = new cHapticDeviceHandler();

	// get a handle to the first haptic device
	handler->getDevice(m_hapticDevice, 0);

	// open a connection to haptic device
	m_hapticDevice->open();

	// calibrate device (if necessary)
	m_hapticDevice->calibrate();


	// create a thread which starts the main haptics rendering loop
	cThread* hapticsThread = new cThread();
	hapticsThread->start(updateHapticsThread, CTHREAD_PRIORITY_HAPTICS);

	// start the main graphics rendering loop
	glutTimerFunc(16, callbackGraphicsTimer, 0);

	return true;
}

void Application::run()
{
	m_timer.start(true);
	glutMainLoop();
}

void Application::resizeWindow(int w, int h)
{
	m_windowWidth = w;
	m_windowHeight = h;
}

void Application::keySelect(unsigned char key, int x, int y)
{
	// option ESC: pop
	if ((key == 27) || (key == 'x'))
	{
		popScene();
	}

	// option f: toggle fullscreen
	if (key == 'f')
	{
		if (m_fullscreen)
		{
			int windowPosX = glutGet(GLUT_INIT_WINDOW_X);
			int windowPosY = glutGet(GLUT_INIT_WINDOW_Y);
			int windowW = glutGet(GLUT_INIT_WINDOW_WIDTH);
			int windowH = glutGet(GLUT_INIT_WINDOW_HEIGHT);
			glutPositionWindow(windowPosX, windowPosY);
			glutReshapeWindow(windowW, windowH);
			m_fullscreen = false;
		}
		else
		{
			glutFullScreen();
			m_fullscreen = true;
		}
	}

	if(!m_sceneStack.empty())
		m_sceneStack.back()->onKeyDown(key, x, y);
}

void Application::updateGraphics(void)	
{  
	m_timer.stop();
	double timeStep = m_timer.getCurrentTimeSeconds();
	m_smoothRenderTimeStep += m_timeStepSmoothFactor * (timeStep - m_smoothRenderTimeStep);
	m_smoothLogicTimeStep += m_timeStepSmoothFactor * (timeStep - m_smoothLogicTimeStep);
	m_timer.start(true);

	if(!m_sceneStack.empty())
	{
		m_sceneStack.back()->updateLogic(timeStep);
		m_sceneStack.back()->render(timeStep);
	}
	else 
	{
		close();
	}

	glutSwapBuffers();

	// check for any OpenGL errors
	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR) 
		std::cerr << "Error:  %s\n" << gluErrorString(err);
}

void Application::graphicsTimer(int data)
{    
	if (m_simulationRunning)
	{
		glutPostRedisplay();
		glutTimerFunc(16, callbackGraphicsTimer, 0);
	}
}

void Application::close()
{
	// stop the simulation
	m_simulationRunning = false;

	// close all scenes
	clearScenes();

	// wait for graphics and haptics loops to terminate
	while (!m_simulationFinished) 
		cSleepMs(100);

	// close haptic device
	m_hapticDevice->close();

	// shut down glut
	glutLeaveMainLoop();
}

void Application::updateHaptics()
{
	// initialize frequency counter
	m_frequencyCounter.reset();

	// simulation in now running
	m_simulationRunning  = true;
	m_simulationFinished = false;

	cPrecisionClock timer;
	timer.start(true);

	// main haptic simulation loop
	while(m_simulationRunning)
	{
		timer.stop();
		double timeStep = timer.getCurrentTimeSeconds();
		m_smoothHapticTimeStep += m_timeStepSmoothFactor * (timeStep - m_smoothHapticTimeStep);
		timer.start(true);

		//Copy to local to avoid threading errors
		ScenePtr current = m_currentScene;
		if(current != nullptr)
			current->updateHaptics(timeStep);

		m_frequencyCounter.signal(1);
	}

	m_simulationFinished = true;
}

void Application::pushScene(ScenePtr scene)
{
	ScenePtr prev = m_sceneStack.empty() ? nullptr : m_sceneStack.back();
	m_sceneStack.push_back(scene);

	if(prev != nullptr)
		prev->exit(scene);
	scene->enter(prev);

	m_currentScene = scene;
}

ScenePtr Application::popScene()
{
	if(m_sceneStack.empty())
		return nullptr;
	else
	{
		ScenePtr top = m_sceneStack.back();
		m_sceneStack.pop_back();

		ScenePtr next = m_sceneStack.empty() ? nullptr : m_sceneStack.back();
		top->exit(next);
		if(next != nullptr)
		{
			next->enter(top);
			m_currentScene = next;
		}
		else
			m_currentScene = nullptr;

		return top;
	}
}

void Application::clearScenes()
{
	while(!m_sceneStack.empty())
		popScene();
}

void Application::callbackResizeWindow(int w, int h)
{
	g_app->resizeWindow(w, h);
}

void Application::callbackKeySelect(unsigned char key, int x, int y)
{
	g_app->keySelect(key, x, y);
}

void Application::callbackUpdateGraphics(void)
{
	g_app->updateGraphics();
}

void Application::callbackGraphicsTimer(int data)
{
	g_app->graphicsTimer(data);
}

void Application::updateHapticsThread()
{
	g_app->updateHaptics();
}