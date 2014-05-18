#pragma once

/**
 * Instances of this type does only cast a shadow
 */
class ShadowlessSphere : public chai3d::cShapeSphere
{
public:
    ShadowlessSphere(const double& a_radius, chai3d::cMaterialPtr a_material = chai3d::cMaterialPtr());
    
    void render(chai3d::cRenderOptions& a_options) override;
    
    void renderSceneGraph(chai3d::cRenderOptions& a_options);
    
};

