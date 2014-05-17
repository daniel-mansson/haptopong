#pragma once

/**
 * Instances of this type does not cast self-shadows
 */
class SelfShadowlessMesh : public chai3d::cMultiMesh
{
    void render(chai3d::cRenderOptions& a_options) override;
    
    void renderSceneGraph(chai3d::cRenderOptions& a_options);
    
};

