#include "pch.h"
#include "ShadowlessSphere.h"

using namespace chai3d;

ShadowlessSphere::ShadowlessSphere(const double& a_radius, cMaterialPtr a_material) :
    cShapeSphere(a_radius,a_material)
{
}

void ShadowlessSphere::render(cRenderOptions& a_options)
{
    if (a_options.m_render_opaque_objects_only) {
        cShapeSphere::render(a_options);
    }
    
    
    //if (a_options.m_render_opaque_objects_only)
    //    cShapeSphere::render(a_options);
    
    //if (a_options.m_creating_shadow_map)
    //    cShapeSphere::render(a_options);
    
    /*if (!a_options.m_creating_shadow_map || a_options.m_rendering_shadow || a_options.m_render_opaque_objects_only)
     {
     //cRenderOptions new_options = a_options;
     //new_options.m_rendering_shadow
     
     a_options.m_shadow_light_level = 1.0;
     
     //a_options.m_render_opaque_objects_only = true;
     //a_options.m_render_textures = true;
     
     cShapeSphere::render(a_options);
     }*/
    
    //if (a_options.m_render_opaque_objects_only)
    //    cShapeSphere::render(a_options);
}

void ShadowlessSphere::renderSceneGraph(cRenderOptions& a_options)
{
#ifdef C_USE_OPENGL
    
    /////////////////////////////////////////////////////////////////////////
    // Initialize rendering
    /////////////////////////////////////////////////////////////////////////
    
    // convert the position and orientation of the object into a 4x4 matrix
    // that is supported by openGL. This task only needs to be performed during the first
    // rendering pass
    if (a_options.m_storeObjectPositions)
    {
        m_frameGL.set(m_localPos, m_localRot);
    }
    
    // push object position/orientation on stack
    glPushMatrix();
    glMultMatrixd( (const double *)m_frameGL.getData() );
    
    // render if object is enabled
    if (m_enabled)
    {
        //--------------------------------------------------------------------------
        // Request for RESET
        //-----------------------------------------------------------------------
        if(a_options.m_resetDisplay)
        {
            // invalidate display list
            invalidateDisplayList();
            
            // invalidate texture
            if (m_texture != nullptr)
            {
                m_texture->markForUpdate();
            }
        }
        
        //-----------------------------------------------------------------------
        // Init
        //-----------------------------------------------------------------------
        
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glEnable(GL_DEPTH_TEST);
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        
        //-----------------------------------------------------------------------
        // Render bounding box, frame, collision detector. (opaque components)
        //-----------------------------------------------------------------------
        if (SECTION_RENDER_OPAQUE_PARTS_ONLY(a_options) && (!a_options.m_rendering_shadow))
        {
            // disable lighting
            glDisable(GL_LIGHTING);
            
            // render boundary box
            if (m_showBoundaryBox)
            {
                // set size on lines
                glLineWidth(1.0);
                
                // set color of boundary box
                glColor4fv(s_boundaryBoxColor.pColor());
                
                // draw box line
                cDrawWireBox(m_boundaryBoxMin(0) , m_boundaryBoxMax(0) ,
                             m_boundaryBoxMin(1) , m_boundaryBoxMax(1) ,
                             m_boundaryBoxMin(2) , m_boundaryBoxMax(2) );
            }
            
            // render collision tree
            if (m_showCollisionDetector && (m_collisionDetector != NULL))
            {
                m_collisionDetector->render(a_options);
            }
            
            // enable lighting
            glEnable(GL_LIGHTING);
            
            // render frame
            if (m_showFrame)
            {
                // set rendering properties
                glPolygonMode(GL_FRONT, GL_FILL);
                
                // draw frame
                cDrawFrame(m_frameSize, m_frameThicknessScale, true);
            }
        }
        
        //-----------------------------------------------------------------------
        // Render graphical representation of object
        //-----------------------------------------------------------------------
        if (m_showEnabled)
        {
            // set polygon and face mode
            glPolygonMode(GL_FRONT_AND_BACK, m_triangleMode);
            
            
            /////////////////////////////////////////////////////////////////////
            // CREATING SHADOW DEPTH MAP
            /////////////////////////////////////////////////////////////////////
            if (a_options.m_creating_shadow_map)
            {
                 glEnable(GL_CULL_FACE);
                 glCullFace(GL_FRONT);
                 
                 // render object
                 render(a_options);
                 glDisable(GL_CULL_FACE);
            }
            
            /////////////////////////////////////////////////////////////////////
            // SINGLE PASS RENDERING
            /////////////////////////////////////////////////////////////////////
            else if (a_options.m_single_pass_only)
            {
                if (m_cullingEnabled)
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                }
                else
                {
                    glDisable(GL_CULL_FACE);
                }
                
                if (m_useTransparency)
                {
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glDepthMask(GL_FALSE);
                }
                else
                {
                    glDisable(GL_BLEND);
                    glDepthMask(GL_TRUE);
                }
                
                // render object
                render(a_options);
                
                // disable blending
                glDisable(GL_BLEND);
                glDepthMask(GL_TRUE);
            }
            
            
            /////////////////////////////////////////////////////////////////////
            // MULTI PASS RENDERING
            /////////////////////////////////////////////////////////////////////
            else
            {
                // opaque objects
                if (a_options.m_render_opaque_objects_only)
                {
                    if (m_cullingEnabled)
                    {
                        glEnable(GL_CULL_FACE);
                        glCullFace(GL_BACK);
                    }
                    else
                    {
                        glDisable(GL_CULL_FACE);
                    }
                    
                    render(a_options);
                }
                
                // render transparent back triangles
                if (a_options.m_render_transparent_back_faces_only)
                {
                    if (m_useTransparency)
                    {
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glDepthMask(GL_FALSE);
                    }
                    else
                    {
                        glDisable(GL_BLEND);
                        glDepthMask(GL_TRUE);
                    }
                    
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_FRONT);
                    
                    render(a_options);
                    
                    // disable blending
                    glDisable(GL_BLEND);
                    glDepthMask(GL_TRUE);
                }
                
                // render transparent front triangles
                if (a_options.m_render_transparent_front_faces_only)
                {
                    if (m_useTransparency)
                    {
                        glEnable(GL_BLEND);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        glDepthMask(GL_FALSE);
                    }
                    else
                    {
                        glDisable(GL_BLEND);
                        glDepthMask(GL_TRUE);
                    }
                    
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                    
                    render(a_options);
                    
                    // disable blending
                    glDisable(GL_BLEND);
                    glDepthMask(GL_TRUE);
                }
            }
        }
    }
    
    // render children
    for (unsigned int i=0; i<m_children.size(); i++)
    {
        m_children[i]->renderSceneGraph(a_options);
    }
    
    // pop current matrix
    glPopMatrix();
    
#endif
}