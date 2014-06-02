#pragma once

/**
 * MultiMesh with ability toggle shadowcasting and/or recieving shadow. W00t!
 */
class CustomMesh : public chai3d::cMultiMesh
{
public:
    CustomMesh(chai3d::cWorld* a_world, bool castShadow  = true, bool recieveShadow = true);
    
    virtual void renderSceneGraph(chai3d::cRenderOptions& a_options) override;
    void renderSceneGraphImpl(chai3d::cRenderOptions& a_options);
    
    chai3d::cWorld* m_worldParent;
    bool m_castShadow;
    bool m_recieveShadow;
};

