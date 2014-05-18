#pragma once

/**
 * Instances of this type does only cast a shadow
 */
class ShadowSphere : public chai3d::cShapeCylinder
{
public:
    ShadowSphere(const double a_baseRadius,
                 const double a_topRadius,
                 const double a_height,
                 chai3d::cMaterialPtr a_material = chai3d::cMaterialPtr());
    
    void render(chai3d::cRenderOptions& a_options) override;
    
};

