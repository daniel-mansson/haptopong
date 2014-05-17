#pragma once

/**
 * Instances of this type does not cast self-shadows
 */
class SelfShadowlessSphere : public chai3d::cShapeSphere
{
public:
    SelfShadowlessSphere(const double& a_radius, chai3d::cMaterialPtr a_material = chai3d::cMaterialPtr());
    
    void render(chai3d::cRenderOptions& a_options) override;
    
    void renderSceneGraph(chai3d::cRenderOptions& a_options);
    
};

