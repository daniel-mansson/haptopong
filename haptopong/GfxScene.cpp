#include "pch.h"
#include "GfxScene.h"

#include "Application.h"

using namespace chai3d;

GfxScene::GfxScene(Application& app) :
Scene(app)
{
    /////////////////////////////////////////////////////////////////////////
    // WORLD & CAMERA
    /////////////////////////////////////////////////////////////////////////
    
	// create a new world.
    m_world = new cWorld();
    
    // set the background color of the environment
    m_world->m_backgroundColor.setGrayLevel(0.5);
    
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
    // TABLE
    /////////////////////////////////////////////////////////////////////////
    
    // create cMultiMesh
    m_table = new cMultiMesh();
    
    // load an object0 file
    bool fileload;
    fileload = m_table->loadFromFile("../../gfx/table.obj");
    
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // add gfx objects to world
    m_world->addChild(m_table);
    
    // Since we don't need to see our polygons from both sides, we enable culling.
    m_table->setUseCulling(true);
    
    // enable display list for faster graphic rendering
    m_table->setUseDisplayList(true, true); // ???
    
    // create texture
    cTexture2dPtr table_texture = cTexture2d::create();
    table_texture->setWrapMode(GL_REPEAT);
    fileload = table_texture->loadFromFile("../../gfx/table_diffuse.png");
    if (!fileload)
    {
        std::cout << "Error - Texture image failed to load correctly." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    m_table->setTexture(table_texture);
    m_table->setUseTexture(true, true);
    
    
    /////////////////////////////////////////////////////////////////////////
    // NET
    /////////////////////////////////////////////////////////////////////////
    
    // create cMultiMesh
    m_net = new cMultiMesh();
    
    // load an object0 file
    fileload = m_net->loadFromFile("../../gfx/net.obj");
    
    if (!fileload)
    {
        std::cout << "Error - 3D Model failed to load correctly" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    // add gfx objects to world
    m_world->addChild(m_net);
    
    // Since we don't need to see our polygons from both sides, we enable culling.
    m_net->setUseCulling(false);

    // enable display list for faster graphic rendering
    m_net->setUseDisplayList(true, true); // ???
    
    // create texture
    cTexture2dPtr net_texture = cTexture2d::create();
    table_texture->setWrapMode(GL_REPEAT);
    fileload = net_texture->loadFromFile("../../gfx/net_diffuse.png");
    if (!fileload)
    {
        std::cout << "Error - Texture image failed to load correctly." << std::endl;
        std::exit(EXIT_FAILURE);
    }
    
    m_net->getMesh(0)->setTexture(net_texture);
    m_net->getMesh(0)->setUseTexture(true, true);
    
    // enable mipmaps for adaptive texture size rendering
     m_net->getMesh(0)->m_texture->setUseMipmaps(true);
    
    // enable transparency for this object
    m_net->getMesh(0)->m_texture->m_image->setTransparentColor(30, 30, 30, 0);
    m_net->getMesh(0)->setUseTransparency(true);
    
    
    /////////////////////////////////////////////////////////////////////////
    // RACKET
    /////////////////////////////////////////////////////////////////////////
    
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

