#pragma once

#include "ShadowSphere.h"

/**
 * Instances of this type does only cast a shadow
 */
class CustomSphere : public chai3d::cShapeSphere
{
public:
    CustomSphere(chai3d::cWorld* world, const double& a_radius, chai3d::cMaterialPtr a_material = chai3d::cMaterialPtr());
    
    void render(chai3d::cRenderOptions& a_options) override;
    
private:
	ShadowSphere* m_shadowShape;
    
};

