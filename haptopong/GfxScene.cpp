#include "pch.h"
#include "GfxScene.h"

#include "Application.h"

using namespace chai3d;

GfxScene::GfxScene(Application& app) :
	Scene(app),
	m_playerTransparency(0.5)
{
    /////////////////////////////////////////////////////////////////////////
    // WORLD & CAMERA
    /////////////////////////////////////////////////////////////////////////
    
	// create a new world.
    m_world = new cWorld();
    
    // set the background color of the environment
    m_world->m_backgroundColor.setGrayLevel(0.6);
    
    // create a camera and insert it into the virtual world
    m_camera = new cCamera(m_world);
    m_world->addChild(m_camera);
    
    // position and orient the camera
    m_camera->set(cVector3d (2.47, 0.0, 0.95),    // camera position (eye)
                  cVector3d (0.0, 0.0, 0.01),    // look at position (target)
                  cVector3d (0.0, 0.0, 1.0));   // direction of the (up) vector
    
    // set the near and far clipping planes of the camera
    m_camera->setClippingPlanes(0.01, 100.0);
    
    // set stereo mode
	m_camera->setStereoMode(C_STEREO_DISABLED);
    
    // set stereo eye separation and focal length (applies only if stereo is enabled)
    m_camera->setStereoEyeSeparation(0.01);
    m_camera->setStereoFocalLength(0.5);
    
    // set vertical mirrored display mode
    m_camera->setMirrorVertical(false);
    
    // enable multi-pass rendering to handle transparent objects
    m_camera->setUseMultipassTransparency(true);
    
    // enable shadow casting
    //m_camera->setUseShadowCasting(true);
    
    
    /////////////////////////////////////////////////////////////////////////
    // LIGHTS
    /////////////////////////////////////////////////////////////////////////
    
    // create a directional light source
    m_light = new cDirectionalLight(m_world);
    
    // insert light source inside world
    m_world->addChild(m_light);
    
    // enable light source
    m_light->setEnabled(true);
    
    // define direction of light beam
    m_light->setDir(-7 , 5, -3);
    
    // set lighting conditions
    m_light->m_ambient.set(0.4, 0.4, 0.4);
    m_light->m_diffuse.set(0.45, 0.45, 0.45);
    m_light->m_specular.set(0.2, 0.2, 0.2);
    
    
    // create a spot light source
    cSpotLight *light = new cSpotLight(m_world);
    
    // attach light to camera
    //m_world->addChild(light);
    
    // enable light source
    light->setEnabled(true);
    
    // position the light source
    light->setLocalPos(2.5, 0.0, 2.0);
    
    // define the direction of the light beam
    light->setDir(-1.0, 0.0, -1.0);
    
    // enable this light source to generate shadows
    light->setShadowMapEnabled(true);
    
    // set the resolution of the shadow map
    light->m_shadowMap->setResolutionMedium();
    
    // set light cone half angle
    light->setCutOffAngleDeg(25);
    
    // set lighting conditions
    light->m_ambient.set(0.0, 0.0, 0.0);
    light->m_diffuse.set(0.9, 0.9, 0.9);
    light->m_specular.set(0.7, 0.7, 0.7);
    
    
    /////////////////////////////////////////////////////////////////////////
    // LABELS
    /////////////////////////////////////////////////////////////////////////
    
	cFont *font = NEW_CFONTCALIBRI20();
    
    // create a label to display the haptic rate of the simulation
    m_label = new cLabel(font);
    m_label->m_fontColor.setBlack();
    m_camera->m_frontLayer->addChild(m_label);
    
	m_label->setString ("GfxScene");
	m_label->setLocalPos(10, 10);
    
    
    /////////////////////////////////////////////////////////////////////////
    // HAPTIC DEVICES / TOOLS
    /////////////////////////////////////////////////////////////////////////
    
    app.getHapticDevice()->close();
    
    // create a haptic device handler
    m_handler = new cHapticDeviceHandler();
    
    // get access to the first available haptic device found
    m_handler->getDevice(m_hapticDevice, 0);
    
    // retrieve information about the current haptic device
    cHapticDeviceInfo hapticDeviceInfo = m_hapticDevice->getSpecifications();
    
    // create a tool (cursor) and insert into the world
    m_tool = new cToolCursor(m_world);
    m_world->addChild(m_tool);
    
    // connect the haptic device to the virtual tool
    m_tool->setHapticDevice(m_hapticDevice);
    
    // define the radius of the tool (sphere)
    //double toolRadius = 0.01;
    
    // define a radius for the tool
    //tool->setRadius(toolRadius);
    
    // hide the device sphere. only show proxy.
    m_tool->setShowContactPoints(/*true*/ false, false);
    
    // create a white cursor
    //m_tool->m_hapticPoint->m_sphereProxy->m_material->setWhite();
    
    // enable if objects in the scene are going to rotate of translate
    // or possibly collide against the tool. If the environment
    // is entirely static, you can set this parameter to "false"
    //m_tool->enableDynamicObjects(true);
    
    // map the physical workspace of the haptic device to a larger virtual workspace.
    m_tool->setWorkspaceRadius(1.0);
    
    // start the haptic tool
    m_tool->start();
    
    
    /////////////////////////////////////////////////////////////////////////
    // TABLE
    /////////////////////////////////////////////////////////////////////////
    
    // create cMultiMesh
    m_table = new cMultiMesh();
    
    // load an object file
    bool fileload;
    fileload = m_table->loadFromFile("../gfx/table.obj");
    
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // add gfx objects to world
    m_world->addChild(m_table);
    
    // Since we don't need to see our polygons from both sides, we enable culling.
    m_table->setUseCulling(true);
    
    // enable display list for faster graphic rendering ???
    m_table->setUseDisplayList(true, true);
    
    // create texture
    cTexture2dPtr table_texture = cTexture2d::create();
    table_texture->setWrapMode(GL_REPEAT);
    fileload = table_texture->loadFromFile("../gfx/table_diffuse.png");
    if (!fileload)
    {
        std::cout << "Error - Texture image failed to load correctly." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    m_table->setTexture(table_texture);
    m_table->setUseTexture(true, true);
    
    // enable mipmaps for adaptive texture size rendering
    //m_net->getMesh(0)->m_texture->setUseMipmaps(true);
    
    
    /////////////////////////////////////////////////////////////////////////
    // NET
    /////////////////////////////////////////////////////////////////////////
    
    // create cMultiMesh
    m_net = new cMultiMesh();
    
    // load an object file
    fileload = m_net->loadFromFile("../gfx/net.obj");
    
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // add gfx objects to world
    m_world->addChild(m_net);
    
    // Since we don't need to see our polygons from both sides, we enable culling.
    m_net->setUseCulling(false);

    // enable display list for faster graphic rendering ???
    m_net->setUseDisplayList(true, true);
    
    // create texture
    cTexture2dPtr net_texture = cTexture2d::create();
    table_texture->setWrapMode(GL_REPEAT);
    fileload = net_texture->loadFromFile("../gfx/net_diffuse.png");
    if (!fileload)
    {
        std::cout << "Error - Texture image failed to load correctly." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    m_net->getMesh(0)->setTexture(net_texture);
    m_net->getMesh(0)->setUseTexture(true, true);
    
    // enable mipmaps for adaptive texture size rendering
    //m_net->getMesh(0)->m_texture->setUseMipmaps(true);
    
    // enable transparency for this object
    m_net->getMesh(0)->m_texture->m_image->setTransparentColor(30, 30, 30, 0);
    m_net->getMesh(0)->setUseTransparency(true);
    
    // m_net->setGfxMesh(...);
    // m_net->setPhysicsMesh(...)
    
    
    /////////////////////////////////////////////////////////////////////////
    // PLAYER RACKET
    /////////////////////////////////////////////////////////////////////////
    
    // create a new mesh.
    m_playerRacket = new cMultiMesh();
    
    // load an object file
    fileload = m_playerRacket->loadFromFile("../gfx/racket.obj");
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // remove the collision detector. we do not want to compute any
    // force feedback rendering on the object itself.
    //m_playerRacket->deleteCollisionDetector(true);
    
    // attach racket to tool
    m_tool->m_image->addChild(m_playerRacket);
    
    // set the position of the racket
    m_tool->setLocalPos(1.9, 0, 0.6);
    
    // rotate the racket in scene
    m_playerRacket->rotateExtrinsicEulerAnglesDeg(40, 0, 0, C_EULER_ORDER_XYZ);
    
    // set transparency
    m_playerRacket->setUseTransparency(true);
    m_playerRacket->setTransparencyLevel(m_playerTransparency);

    
    /////////////////////////////////////////////////////////////////////////
    // OPPONENT RACKET
    /////////////////////////////////////////////////////////////////////////
    
    // create a new mesh.
    m_opponentRacket = new cMultiMesh();
    
    // load an object file
    fileload = m_opponentRacket->loadFromFile("../gfx/racket.obj");
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // remove the collision detector. we do not want to compute any
    // force feedback rendering on the object itself.
    //m_opponentRacket->deleteCollisionDetector(true);
    
    // add gfx objects to world
    m_world->addChild(m_opponentRacket);
    
    // Since we don't need to see our polygons from both sides, we enable culling.
    m_opponentRacket->setUseCulling(false);
    
    // set the position of the racket
    m_opponentRacket->setLocalPos(-1.9, 0, 0.6);
    
    // rotate the racket in scene
    m_opponentRacket->rotateExtrinsicEulerAnglesDeg(-40, 0, 0, C_EULER_ORDER_XYZ);
    
    cMaterial mat;
    mat.m_ambient.set(0.8, 0.8, 0.8);
    mat.m_diffuse.set(1.0, 1.0, 1.0);
    mat.m_specular.set(1.0, 1.0, 1.0);
    m_opponentRacket->setMaterial(mat, true);
    m_opponentRacket->computeAllNormals();
    
    
    /////////////////////////////////////////////////////////////////////////
    // BALL
    /////////////////////////////////////////////////////////////////////////
    
    m_ball = new cShapeSphere(0.02);
	m_world->addChild(m_ball);
    
    // set the position of the ball
    m_ball->setLocalPos(1.7, 0, 0.5);
    
}

GfxScene::~GfxScene(void)
{
}

void GfxScene::enter(ScenePtr from)
{
}

void GfxScene::exit(ScenePtr to)
{
}

void GfxScene::render(const double& timeStep)
{
	m_label->setString ("GfxScene " + cStr(m_app.getHapticHz()) + "   " + cStr(1.0 / m_app.getSmoothRenderTimeStep(), 0));
    
	m_camera->renderView(m_app.getWindowWidth(), m_app.getWindowHeight());
}

void GfxScene::updateLogic(const double& timeStep)
{
}

void GfxScene::updateHaptics(const double& timeStep)
{
}

void GfxScene::onKeyDown(unsigned char key, int x, int y)
{
}

