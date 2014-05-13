#pragma once

/**
 * cMultiMesh with overriden render method. Instances of this
 * type does not cast shadows
 */
class ShadowlessMesh : public chai3d::cMultiMesh
{
    void render(chai3d::cRenderOptions& a_options) override;
    
public:
    ShadowlessMesh* copy(const bool a_duplicateMaterialData = false,
                         const bool a_duplicateTextureData = false,
                         const bool a_duplicateMeshData = false,
                         const bool a_buildCollisionDetector = true);
};

