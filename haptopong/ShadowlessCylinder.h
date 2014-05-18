#pragma once

class ShadowlessCylinder : public chai3d::cShapeCylinder
{
public:
    ShadowlessCylinder(const double a_baseRadius, 
                   const double a_topRadius, 
                   const double a_height, 
                   chai3d::cMaterialPtr a_material = chai3d::cMaterialPtr());
    
    void render(chai3d::cRenderOptions& a_options) override;
    
    void renderSceneGraph(chai3d::cRenderOptions& a_options);
    
};

