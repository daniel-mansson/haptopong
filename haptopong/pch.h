
#include <chai3d.h>
#include <vector>
#include <iostream>
#include <cstdlib>

#include <GL/freeglut.h>

//Chai ODE
#include <CODE.h>

#ifndef MACOSX
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

namespace chai3d
{
	typedef std::shared_ptr<cWorld> cWorldPtr;
	typedef std::shared_ptr<cFont> cFontPtr;
};
