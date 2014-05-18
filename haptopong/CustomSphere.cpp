#include "pch.h"
#include "CustomSphere.h"

using namespace chai3d;

CustomSphere::CustomSphere(cWorld* world, const double& a_radius, cMaterialPtr a_material) :
    cShapeSphere(a_radius,a_material)
{
    
    m_shadowShape = new ShadowSphere(a_radius, a_radius, 0.);
    m_shadowShape->setUseCulling(true);
    
    world->addChild(m_shadowShape);
}

void CustomSphere::render(cRenderOptions& a_options)
{
    if (a_options.m_render_opaque_objects_only) {
        cShapeSphere::render(a_options);
    }
    
    chai3d::cVector3d pos = getLocalPos();
    
    m_shadowShape->setLocalPos(pos.x(), pos.y(), pos.z() - getRadius()+0.001);

}
