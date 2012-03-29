/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        renderer.cpp
// PURPOSE:     OpenGL3 renderer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <sstream>
#include "opengl3.h"
#include "renderer/vertex_weight.h"

using namespace Renderer;

OpenGL3_Renderer::OpenGL3_Renderer()
: m_initialized(false)
, m_program(0)
, m_texLookup ( 0 )
{
    strcpy ( m_error, "Success" );
}

OpenGL3_Renderer::~OpenGL3_Renderer()
{
}

bool OpenGL3_Renderer::initialize()
{
    if ( m_initialized )
        return true;

    // Initialize GLEW
    if ( glewInit () != 0 )
            return false;

    strcpy ( m_error, "Success" );

    m_initialized = ( strcmp(m_error, "Success") == 0 );
    return m_initialized;
}

void OpenGL3_Renderer::setProgram ( IProgram* program )
{
    if ( m_program != program )
    {
        m_program = program;
    }
}

static Matrix getBasisChanger ()
{
    // Change the basis to the OpenGL basis
    static const f32 m [ 16 ] = {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,  -1.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
    static const Matrix s_matBasisChanger ( m );
    return s_matBasisChanger;
}

bool OpenGL3_Renderer::beginScene ( const Matrix& matProjection, const Matrix& matLookat, TextureLookupFn texLookup )
{
    glEnable ( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable ( GL_DEPTH_TEST );
    
    const f32* col0 = &matProjection.m[0][0];
    const f32* col1 = &matProjection.m[1][0];
    const f32* col2 = &matProjection.m[2][0];
    const f32* col3 = &matProjection.m[3][0];
    const f32 projectionM [ 16 ] = {
         col0[0],  col0[2], -col0[1],  col0[3],
         col2[0],  col2[2], -col2[1],  col2[3],
        -col1[0], -col1[2], -col1[1], -col1[3],
         col3[0],  col3[2], -col3[1],  col3[3]
    };
    
    m_matProjection = Matrix ( projectionM );
    m_matLookat = matLookat;
    
    m_viewVector = Vector3 ( 0.0f, 1.0f, 0.0 ) * ( getBasisChanger() * m_matLookat );
    m_viewVector.normalize();
    
    m_texLookup = texLookup;
    
    return true;
}

void OpenGL3_Renderer::setupLighting()
{
    // For now, use a light attached to the camera
    Vector3 pos ( 0, 0, 0 );
    Vector3 dir ( 0, 1, 0 );
    pos = m_matLookat * pos;
    dir = Matrix2Transform(m_matLookat).orientation() * dir;

    m_program->setUniform("un_Lights[0].diffuse", Color(255, 255, 255, 255), false );
    m_program->setUniform("un_Lights[0].ambient", Color(10, 20, 25, 255), false );
    m_program->setUniform("un_Lights[0].specular", Color(255, 255, 255, 255), false );
    m_program->setUniform("un_Lights[0].position", pos );
    m_program->setUniform("un_Lights[0].direction", dir );
}

bool OpenGL3_Renderer::render ( Geometry* geometry, const Transform& transform, bool includeTransparent, MeshRenderFn fn )
{
    if ( m_program == 0 || m_program->ok() == false )
    {
        if ( !m_program )
            strcpy ( m_error, "Invalid program" );
        else
            m_program->getError ( m_error );
        return false;
    }
    if ( !m_program->use () )
    {
        m_program->getError ( m_error );
        return false;
    }
        
    if ( !geometry->initialized() )
        if ( !geometry->initialize() )
            return false;
    
    setupLighting ();
 
    Matrix lookAt = getBasisChanger() * m_matLookat;
    Matrix mat = Transform2Matrix ( transform );
    Matrix matNormals = MatrixForNormals ( mat );
    Matrix matGeometry = m_matProjection * lookAt * mat;

    // Use vertex buffers
    const Vertex* v = 0;
    glBindBuffer ( GL_ARRAY_BUFFER, geometry->m_vertexBuffer );
    
    glVertexAttribPointer ( OpenGL3_Program::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLchar *)&(v->pos()) );
    eglGetError();
    glVertexAttribPointer ( OpenGL3_Program::NORMAL, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLchar *)&(v->norm()) );
    eglGetError();
    glEnableVertexAttribArray ( OpenGL3_Program::POSITION );
    eglGetError();
    glEnableVertexAttribArray ( OpenGL3_Program::NORMAL );
    eglGetError();
    
                    
    // Setup the skinning
    b8 doSkinning = false;
    Matrix poseMatrices [ Pose::MAX_JOINTS ];
    
    VertexWeightSOA* weightsSOA = geometry->getVertexLayer<VertexWeightSOA>("weights", 0);
    if ( geometry->pose() != 0 && weightsSOA != 0 )
    {
        u32 weights = 0;
        u32 joints = sizeof(float) * VertexWeight::MAX_ASSOCIATIONS;

        if ( geometry->bindVertexLayer(m_program, "in_VertexWeight", "weights", 0, Geometry::FLOAT, false, VertexWeight::MAX_ASSOCIATIONS, weights) )
        {
            if ( geometry->bindVertexLayer(m_program, "in_Joint", "weights", 0, Geometry::FLOAT, false, VertexWeight::MAX_ASSOCIATIONS, joints ) )
            {
                doSkinning = true;
                geometry->pose()->calculateTransforms( &poseMatrices[0] );
            }
        }
    }
    
    for ( Geometry::meshNodeVector::iterator iter = geometry->m_meshNodes.begin();
          iter != geometry->m_meshNodes.end();
          ++iter )
    {
        Mesh* mesh = (*iter).mesh;
        Material* material = mesh->material();
        
        if ( !includeTransparent && material != 0 && material->isTransparent() == true )
            continue;
        
        if ( !fn || fn(mesh) == true )
        {
            // Set the uniforms
            m_program->setUniform("un_ProjectionMatrix", m_matProjection );
            m_program->setUniform("un_LookatMatrix", lookAt );
            m_program->setUniform("un_ModelviewMatrix", mat);
            m_program->setUniform("un_NormalMatrix", matNormals);
            m_program->setUniform("un_Matrix", matGeometry );
            m_program->setUniform("un_ViewVector", m_viewVector );
            
            // Setup skinning
            if ( doSkinning )
            {
                m_program->setUniform ( "un_JointMatrices", &poseMatrices[0], geometry->pose()->numJoints() ); 
                m_program->setUniform("un_Skinning", true);
            }
            else
                m_program->setUniform("un_Skinning", false);
    
            GLenum polyType = GL_INVALID_ENUM;
            switch ( mesh->polyType() )
            {
                case Mesh::TRIANGLES: polyType = GL_TRIANGLES; break;
                case Mesh::TRIANGLE_STRIP: polyType = GL_TRIANGLE_STRIP; break;
                case Mesh::TRIANGLE_FAN: polyType = GL_TRIANGLE_FAN; break;
                case Mesh::QUADS: polyType = GL_QUADS; break;
                default: break;
            }

            if ( polyType != GL_INVALID_ENUM )
            {
                // Set the material
                i32 textureLevels = 0;
                
                if ( material != 0 )
                {
                    m_program->setUniform( "un_Material.diffuse", material->diffuse(), true );
                    m_program->setUniform( "un_Material.ambient", material->ambient(), false );
                    m_program->setUniform( "un_Material.specular", material->specular(), false );
                    m_program->setUniform( "un_Material.emission", material->emission(), false );
                    m_program->setUniform( "un_Material.shininess", material->shininess() );
                    m_program->setUniform( "un_Material.isShadeless", material->isShadeless() );
                    
                    // Setup the texturing
                    b8 doTexturing = false;
                    if ( m_texLookup != 0 && material->texture() != "" )
                    {
                        ITexture* tex = m_texLookup ( material->texture() );
                        if ( tex != 0 && tex->bind() &&
                             geometry->bindVertexLayer(m_program, "in_TexCoord", "uv", 0, Geometry::FLOAT, false, 2, 0) )
                        {
                            doTexturing = true;
                        }
                    }
                    
                    if ( doTexturing )
                    {
                        textureLevels = 1;
                        m_program->setUniform( "un_Sampler0", 0 );
                    }
                    else
                    {
                        textureLevels = 0;
                        geometry->unbindAttribute(m_program, "in_TexCoord");
                    }
                }
                else
                {
                    m_program->setUniform( "un_Material.diffuse", Color(255*0.7f, 255*0.7f, 255*0.7f, 255) );
                    m_program->setUniform( "un_Material.ambient", Vector3(0.7f, 0.7f, 0.7f) );
                    m_program->setUniform( "un_Material.specular", Vector3(0.0f, 0.0f, 0.0f) );
                    m_program->setUniform( "un_Material.emission", Vector3(0.0f, 0.0f, 0.0f) );
                    m_program->setUniform( "un_Material.shininess", 0.0f );
                    m_program->setUniform( "un_Material.isShadeless", false );
                    textureLevels = 0;
                    geometry->unbindAttribute(m_program, "in_TexCoord");
                }
                
                m_program->setUniform( "un_TextureLevels", (f32)textureLevels );
                
                // Bind the indices buffer
                glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, geometry->m_elementBuffer );
                eglGetError();
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDrawElements ( polyType, mesh->numIndices(), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>((*iter).offset * sizeof(u32)) );
                eglGetError();
            }
        }
    }
    
    return true;
}

// TODO: Make common factor with render
bool OpenGL3_Renderer::renderGeometryMesh(Geometry* geometry, Mesh* mesh, const Transform& transform, MeshRenderFn fn)
{
    if ( m_program == 0 || m_program->ok() == false )
    {
        if ( !m_program )
            strcpy ( m_error, "Invalid program" );
        else
            m_program->getError ( m_error );
        return false;
    }
    if ( !m_program->use () )
    {
        m_program->getError ( m_error );
        return false;
    }
        
    if ( !geometry->initialized() )
        if ( !geometry->initialize() )
            return false;
    
    setupLighting ();
    
    Matrix lookAt = getBasisChanger() * m_matLookat;
    Matrix mat = Transform2Matrix ( transform );
    Matrix matNormals = MatrixForNormals ( mat );
    Matrix matGeometry = m_matProjection * lookAt * mat;

    // Use vertex buffers
    const Vertex* v = 0;
    glBindBuffer ( GL_ARRAY_BUFFER, geometry->m_vertexBuffer );
    
    glVertexAttribPointer ( OpenGL3_Program::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLchar *)&(v->pos()) );
    eglGetError();
    glVertexAttribPointer ( OpenGL3_Program::NORMAL, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLchar *)&(v->norm()) );
    eglGetError();
    glEnableVertexAttribArray ( OpenGL3_Program::POSITION );
    eglGetError();
    glEnableVertexAttribArray ( OpenGL3_Program::NORMAL );
    eglGetError();
    
                    
    // Setup the skinning
    b8 doSkinning = false;
    Matrix poseMatrices [ Pose::MAX_JOINTS ];
    
    VertexWeightSOA* weightsSOA = geometry->getVertexLayer<VertexWeightSOA>("weights", 0);
    if ( geometry->pose() != 0 && weightsSOA != 0 )
    {
        u32 weights = 0;
        u32 joints = sizeof(float) * VertexWeight::MAX_ASSOCIATIONS;

        if ( geometry->bindVertexLayer(m_program, "in_VertexWeight", "weights", 0, Geometry::FLOAT, false, VertexWeight::MAX_ASSOCIATIONS, weights) )
        {
            if ( geometry->bindVertexLayer(m_program, "in_Joint", "weights", 0, Geometry::FLOAT, false, VertexWeight::MAX_ASSOCIATIONS, joints ) )
            {
                doSkinning = true;
                geometry->pose()->calculateTransforms( &poseMatrices[0] );
            }
        }
    }
    
    for ( Geometry::meshNodeVector::iterator iter = geometry->m_meshNodes.begin();
          iter != geometry->m_meshNodes.end();
          ++iter )
    {
        Mesh* m = (*iter).mesh;
        if ( m != mesh )
            continue;

        Material* material = mesh->material();
        
        if ( !fn || fn(mesh) == true )
        {
            // Set the uniforms
            m_program->setUniform("un_ProjectionMatrix", m_matProjection );
            m_program->setUniform("un_LookatMatrix", lookAt );
            m_program->setUniform("un_ModelviewMatrix", mat);
            m_program->setUniform("un_NormalMatrix", matNormals);
            m_program->setUniform("un_Matrix", matGeometry );
            m_program->setUniform("un_ViewVector", m_viewVector );
            
            // Setup skinning
            if ( doSkinning )
            {
                m_program->setUniform ( "un_JointMatrices", &poseMatrices[0], geometry->pose()->numJoints() ); 
                m_program->setUniform("un_Skinning", true);
            }
            else
                m_program->setUniform("un_Skinning", false);
    
            GLenum polyType = GL_INVALID_ENUM;
            switch ( mesh->polyType() )
            {
                case Mesh::TRIANGLES: polyType = GL_TRIANGLES; break;
                case Mesh::TRIANGLE_STRIP: polyType = GL_TRIANGLE_STRIP; break;
                case Mesh::TRIANGLE_FAN: polyType = GL_TRIANGLE_FAN; break;
                case Mesh::QUADS: polyType = GL_QUADS; break;
                default: break;
            }

            if ( polyType != GL_INVALID_ENUM )
            {
                // Set the material
                i32 textureLevels = 0;
                
                if ( material != 0 )
                {
                    m_program->setUniform( "un_Material.diffuse", material->diffuse(), true );
                    m_program->setUniform( "un_Material.ambient", material->ambient(), false );
                    m_program->setUniform( "un_Material.specular", material->specular(), false );
                    m_program->setUniform( "un_Material.emission", material->emission(), false );
                    m_program->setUniform( "un_Material.shininess", material->shininess() );
                    m_program->setUniform( "un_Material.isShadeless", material->isShadeless() );
                    
                    // Setup the texturing
                    b8 doTexturing = false;
                    if ( m_texLookup != 0 && material->texture() != "" )
                    {
                        ITexture* tex = m_texLookup ( material->texture() );
                        if ( tex != 0 && tex->bind() &&
                             geometry->bindVertexLayer(m_program, "in_TexCoord", "uv", 0, Geometry::FLOAT, false, 2, 0) )
                        {
                            doTexturing = true;
                        }
                    }
                    
                    if ( doTexturing )
                    {
                        textureLevels = 1;
                        m_program->setUniform( "un_Sampler0", 0 );
                    }
                    else
                    {
                        textureLevels = 0;
                        geometry->unbindAttribute(m_program, "in_TexCoord");
                    }
                }
                else
                {
                    m_program->setUniform( "un_Material.diffuse", Color(255*0.7f, 255*0.7f, 255*0.7f, 255) );
                    m_program->setUniform( "un_Material.ambient", Vector3(0.7f, 0.7f, 0.7f) );
                    m_program->setUniform( "un_Material.specular", Vector3(0.0f, 0.0f, 0.0f) );
                    m_program->setUniform( "un_Material.emission", Vector3(0.0f, 0.0f, 0.0f) );
                    m_program->setUniform( "un_Material.shininess", 0.0f );
                    m_program->setUniform( "un_Material.isShadeless", false );
                    textureLevels = 0;
                    geometry->unbindAttribute(m_program, "in_TexCoord");
                }
                
                m_program->setUniform( "un_TextureLevels", (f32)textureLevels );
                
                // Bind the indices buffer
                glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, geometry->m_elementBuffer );
                eglGetError();
                //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                glDrawElements ( polyType, mesh->numIndices(), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>((*iter).offset * sizeof(u32)) );
                eglGetError();
            }
        }
    }
    
    return true;
}

bool OpenGL3_Renderer::endScene()
{
#if 0
    // Draw the debug coordinate system
    glDisable ( GL_LIGHTING );
    glUseProgram ( 0 );
    glMatrixMode ( GL_PROJECTION_MATRIX );
    glLoadIdentity ();
    glMatrixMode ( GL_MODELVIEW_MATRIX );
    glLoadMatrixf ( (m_matProjection * m_matLookat).vector() );
    
    glBegin ( GL_LINES );
        glColor3f ( 1.0f, 0.0f, 0.0f );
        glVertex3f ( 0.0f, 0.0f, 0.0f );
        glVertex3f ( 10.0f, 0.0f, 0.0f );
        glColor3f ( 0.0f, 1.0f, 0.0f );
        glVertex3f ( 0.0f, 0.0f, 0.0f );
        glVertex3f ( 0.0f, 10.0f, 0.0f );
        glColor3f ( 0.0f, 0.0f, 1.0f );
        glVertex3f ( 0.0f, 0.0f, 0.0f );
        glVertex3f ( 0.0f, 0.0f, 10.0f );
    glEnd ();
#endif

    glUseProgram ( 0 );
    return true;
}
