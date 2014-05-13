#include "pch.h"
#include "ShadowlessMesh.h"

void ShadowlessMesh::render(chai3d::cRenderOptions& a_options)
{
    chai3d::cRenderOptions newOptions = a_options;
    
    // setup rendering options for first pass
    //newOptions.m_displayContext                        = a_displayContext;
    //newOptions.m_camera                                = this;
    //newOptions.m_single_pass_only                      = false;
    //newOptions.m_render_opaque_objects_only            = false;
    //newOptions.m_render_transparent_front_faces_only   = true;
    //newOptions.m_render_transparent_back_faces_only    = true;
    //newOptions.m_enable_lighting                       = false;
    //newOptions.m_render_materials                      = true;
    //newOptions.m_render_textures                       = true;
    newOptions.m_creating_shadow_map                     = false;
    //newOptions.m_rendering_shadow                      = false;
    //newOptions.m_shadow_light_level                    = 0.0;
    //newOptions.m_storeObjectPositions                  = false;
    //newOptions.m_resetDisplay                          = m_resetDisplay;    
    
    cMultiMesh::render(newOptions);
}

ShadowlessMesh* ShadowlessMesh::copy(const bool a_duplicateMaterialData,
                                     const bool a_duplicateTextureData,
                                     const bool a_duplicateMeshData,
                                     const bool a_buildCollisionDetector)
{
    // create multimesh object
    ShadowlessMesh* obj = new ShadowlessMesh();
    
    // copy multimesh properties
    copyMultiMeshProperties(obj,
                            a_duplicateMaterialData,
                            a_duplicateTextureData,
                            a_duplicateMeshData,
                            a_buildCollisionDetector);
    
    // return reult
    return (obj);
}