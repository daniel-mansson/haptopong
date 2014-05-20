
#ifdef WIN32
#define _VARIADIC_MAX 6
#endif 

//Network engine
#ifndef MACOSX
#include <WinSock2.h>
#endif
#include <enet/enet.h>

#include <chai3d.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <bass.h>
#include <cstring>

#ifndef MACOSX
#include <GL/freeglut.h>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

//Bullet physics engine
#include <btBulletDynamicsCommon.h>
#include <cd_wavefront.h>

#include "Util.h"

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

typedef std::shared_ptr<btRigidBody> btRigidBodyPtr;
typedef std::shared_ptr<btCollisionShape> btCollisionShapePtr;