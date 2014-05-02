
//Network engine
#include <enet/enet.h>

#include <chai3d.h>
#include <vector>
#include <iostream>
#include <cstdlib>

#ifndef MACOSX
#include <GL/freeglut.h>
#endif

//Bullet physics engine
#include <btBulletDynamicsCommon.h>


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
