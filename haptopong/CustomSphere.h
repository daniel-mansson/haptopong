#pragma once

/**
 * cShapeSphere with ability toggle shadowcasting and/or recieving shadow
 */
class CustomSphere : public chai3d::cShapeSphere
{
public:
    CustomSphere(chai3d::cWorld* a_world, const double& a_radius, bool castShadow  = true, bool recieveShadow = true, chai3d::cMaterialPtr a_material = chai3d::cMaterialPtr());
    
    virtual void renderSceneGraph(chai3d::cRenderOptions& a_options) override;
    void renderSceneGraphImpl(chai3d::cRenderOptions& a_options);
    
private:
    chai3d::cWorld* m_worldParent;
    bool m_castShadow;
    bool m_recieveShadow;
    
};

