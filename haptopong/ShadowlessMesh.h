#pragma once

/**
 * cMultiMesh with overriden render method. Instances of this
 * type does not cast shadows
 */
class ShadowlessMesh : public chai3d::cMultiMesh
{
    void render(chai3d::cRenderOptions& a_options) override;
};
