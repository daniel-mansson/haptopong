#include "pch.h"
#include "ShadowSphere.h"

using namespace chai3d;

ShadowSphere::ShadowSphere(const double a_baseRadius,
                           const double a_topRadius,
                           const double a_height,
                           cMaterialPtr a_material) :
cShapeCylinder(a_baseRadius, a_topRadius, a_height, a_material)
{
    //cShapeSphere::setUseTransparency(true);
    //cShapeSphere::setTransparencyLevel(0.f);
}

void ShadowSphere::render(cRenderOptions& a_options)
{
    if (a_options.m_creating_shadow_map)
        cShapeCylinder::render(a_options);
}
