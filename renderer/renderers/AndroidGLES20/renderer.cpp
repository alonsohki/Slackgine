/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2012
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        renderer.cpp
// PURPOSE:     Android OpenGL ES 2.0 renderer.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <sstream>
#include "gles20.h"
#include "renderer/vertex_weight.h"

using namespace Renderer;

GLES20_Renderer::GLES20_Renderer()
: m_initialized(false)
, m_vertexShader(0)
, m_fragmentShader(0)
, m_program(0)
{
    strcpy ( m_error, "Success" );
}

GLES20_Renderer::~GLES20_Renderer()
{
    if ( m_program != 0 )
    {
        sgDelete m_vertexShader;
        sgDelete m_fragmentShader;
        sgDelete m_program;
    }
}

bool GLES20_Renderer::initialize()
{
    if ( m_initialized )
        return true;
    strcpy ( m_error, "Success" );
       
    m_initialized = ( strcmp(m_error, "Success") == 0 );
    return m_initialized;
}

void GLES20_Renderer::setProgram ( IProgram* program )
{
    m_program = program;
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


bool GLES20_Renderer::beginScene ( const Matrix& matProjection, const Matrix& matLookat, TextureLookupFn texLookup )
{
    //glEnable ( GL_CULL_FACE );
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


void GLES20_Renderer::setupLighting()
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


bool GLES20_Renderer::prepareGeometry ( Geometry* geometry, const Transform& transform )
{
    if ( m_program == 0 ) {
        strcpy ( m_error, "Invalid program" );
        return false;
    }
    if ( !m_program->use() )
    {
        m_program->getError ( m_error );
        return false;
    }
    
    if ( !geometry->initialized() )
        if ( !geometry->initialize() )
            return false;

    
    // make sure all the vertex attribute arrays are disabled
    for ( GLuint index = 0; index < GLES20_Program::MAX_VERTEX_ATTRIBS; index++ ) {
        glDisableVertexAttribArray(index);
    }
    
    
    // setup basic vertex attributes
    glBindBuffer ( GL_ARRAY_BUFFER, geometry->m_vertexBuffer );   
    glVertexAttribPointer ( GLES20_Program::POSITION, 3, GL_FLOAT, GL_FALSE, 
                            sizeof(Vertex), (char *) OFFSETOF(Vertex, pos()) );
    glVertexAttribPointer ( GLES20_Program::NORMAL,   3, GL_FLOAT, GL_TRUE,  
                            sizeof(Vertex), (char *) OFFSETOF(Vertex, norm()) );
    eglGetError();
    glEnableVertexAttribArray ( GLES20_Program::POSITION );
    glEnableVertexAttribArray ( GLES20_Program::NORMAL );
    eglGetError();
    
    
    // setup the matrices
    {
      Matrix lookAt = getBasisChanger() * m_matLookat;
      Matrix mat = Transform2Matrix ( transform );
      Matrix matNormals = MatrixForNormals ( mat );
      Matrix matGeometry = m_matProjection * lookAt * mat;
      
      m_program->setUniform("un_ProjectionMatrix", m_matProjection );
      m_program->setUniform("un_LookatMatrix",     lookAt );
      m_program->setUniform("un_ModelviewMatrix",  mat );
      m_program->setUniform("un_NormalMatrix",     matNormals );
      m_program->setUniform("un_Matrix",           matGeometry );
      m_program->setUniform("un_ViewVector",       m_viewVector );
    }
    
    
    // setup lighting
    setupLighting ();
    
    
    // setup the texture samplers
    m_program->setUniform( "un_Sampler0", 0 );
        
    
    // Setup the skinning
    {
        bool doSkinning = false;
        if ( geometry->pose() != 0 )
        {
            Matrix poseMatrices [ Pose::MAX_JOINTS ];
            u32 offsetWeights = (u32) OFFSETOF(VertexWeightSOA, weight);
            u32 offsetJoints =  (u32) OFFSETOF(VertexWeightSOA, joint);

            doSkinning = true
              && geometry->bindVertexLayer(m_program, "in_VertexWeight", "weights", 0, Geometry::FLOAT, false, VertexWeight::MAX_ASSOCIATIONS, offsetWeights)
              && geometry->bindVertexLayer(m_program, "in_Joint",        "weights", 0, Geometry::FLOAT, false, VertexWeight::MAX_ASSOCIATIONS, offsetJoints);

            if ( doSkinning ) {
                geometry->pose()->calculateTransforms( &poseMatrices[0] );
                m_program->setUniform( "un_JointMatrices", &poseMatrices[0], geometry->pose()->numJoints() ); 
            }
        }

        m_program->setUniform("un_Skinning", doSkinning);
    }

    
    // Setup the morphing
    {
      bool doMorphing = false; 
      if ( geometry->morph() != 0 )
      {      
        Renderer::Morph &morph = *geometry->morph();
        const u32 MAX_ACTIVE_SHAPES = Renderer::Morph::MAX_ACTIVE_SHAPES;
        char attrNamePos[32];
        char attrNameNorm[32];

        // weights of the active shapes (will be loaded to the shader as uniform)
        float weight[MAX_ACTIVE_SHAPES]; 

        u32 numActiveShapes = 0;
        for (u32 i = 0; i<MAX_ACTIVE_SHAPES; i++)
        {
          bool isShapeActive = false;
          snprintf(attrNamePos,  sizeof(attrNamePos),  "in_Shape%d_pos", i);
          snprintf(attrNameNorm, sizeof(attrNameNorm), "in_Shape%d_norm", i);

          // for each active shape bind the attribute array from the vertex layers
          if (i < morph.numActiveShapes())
          {
            u32 shapeNum = morph.activeShapes()[i];
            u32 offsetPos =  (u32) OFFSETOF(Vertex, pos());
            u32 offsetNorm = (u32) OFFSETOF(Vertex, norm());

            isShapeActive = true
              && geometry->bindVertexLayer(m_program, attrNamePos,  "shapes", shapeNum, Geometry::FLOAT, false, 3, offsetPos)
              && geometry->bindVertexLayer(m_program, attrNameNorm, "shapes", shapeNum, Geometry::FLOAT, false, 3, offsetNorm);

            if (isShapeActive) {
              weight[i] = morph.activeWeights()[i];
              numActiveShapes++;
            }
          }

          // unbind the attributes of the innactive shapes
          if (!isShapeActive)
          {
            geometry->unbindAttribute(m_program, attrNamePos);
            geometry->unbindAttribute(m_program, attrNameNorm);

            // weight is forced to 0 to ensure that they will have no effect
            weight[i] = 0.0f;
          }
        }

        // load the weights
        m_program->setUniform("un_ShapeWeight", weight, MAX_ACTIVE_SHAPES);
        doMorphing = (numActiveShapes > 0);
      }
    
      m_program->setUniform("un_Morphing", doMorphing);
    }
    
    return true;
}


bool GLES20_Renderer::drawMeshNode ( Geometry* geometry, u32 meshNum, MeshRenderFn fn )
{        
    Geometry::MeshNode& meshNode = geometry->m_meshNodes[meshNum];
    Mesh* mesh = meshNode.mesh;
    const void* offsetInElementBuffer = reinterpret_cast<const GLvoid *>( meshNode.offset * sizeof(u32) );
    
    // skip meshes that don't pass the MeshRender function
    if ( fn && ! fn(mesh) ) {
      return false;
    }
    
    Material* material = mesh->material();

    // Translate the primitive type
    GLenum polyType = 0;
    switch ( mesh->polyType() )
    {
        case Mesh::TRIANGLES:      polyType = GL_TRIANGLES;      break;
        case Mesh::TRIANGLE_STRIP: polyType = GL_TRIANGLE_STRIP; break;
        case Mesh::TRIANGLE_FAN:   polyType = GL_TRIANGLE_FAN;   break;
        default:
          // ignore meshes with bad primitive type
          return false;
    }
    

    // Setup the material
    Material defaultMaterial( 
        Color(178, 178, 178, 255),  // ambient
        Color(178, 178, 178, 255)); // difuse
    
    if ( material == 0 ) {
      material = &defaultMaterial;
    }   
    m_program->setUniform( "un_Material.diffuse",     material->diffuse(),    true  );
    m_program->setUniform( "un_Material.ambient",     material->ambient(),    false );
    m_program->setUniform( "un_Material.specular",    material->specular(),   false );
    m_program->setUniform( "un_Material.emission",    material->emission(),   false );
    m_program->setUniform( "un_Material.shininess",   material->shininess()   );
    m_program->setUniform( "un_Material.isShadeless", material->isShadeless() );


    // Setup the texturing
    {
        i32 textureLevels = 0;
        if ( m_texLookup != 0 && !material->texture().empty() )
        {
            ITexture* tex = m_texLookup ( material->texture() );
            if ( tex != 0 && tex->bind()
              && geometry->bindVertexLayer(m_program, "in_TexCoord", "uv", 0, Geometry::FLOAT, false, 2, 0) )
            {
                textureLevels = 1;
            }
        }

        if ( textureLevels < 1 )
        {
            geometry->unbindAttribute(m_program, "in_TexCoord");
        }
        m_program->setUniform( "un_TextureLevels", (f32)textureLevels );
    }

    // Render the mesh
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, geometry->m_elementBuffer );
    eglGetError();
    glDrawElements ( polyType, mesh->numIndices(), GL_UNSIGNED_INT, offsetInElementBuffer );
    eglGetError();
    return true;
}


bool GLES20_Renderer::render ( Geometry* geometry, const Transform& transform, bool includeTransparent, MeshRenderFn fn )
{
    if ( ! prepareGeometry(geometry, transform) ) {
        return false;
    }
    
    for ( u32 meshNum=0; meshNum<geometry->m_meshNodes.size(); meshNum++ )
    {
        Mesh* mesh = geometry->m_meshNodes[meshNum].mesh;
        Material* material = mesh->material();
        
        // skip transparent models according if includeTransparent is not set
        if ( !includeTransparent && material != 0 && material->isTransparent() )
            continue;
        
        drawMeshNode( geometry, meshNum, fn );
    }
    
    return true;    
}


bool GLES20_Renderer::renderGeometryMesh ( Geometry* geometry, Mesh* mesh, const Transform& transform, MeshRenderFn fn )
{
    if ( ! prepareGeometry(geometry, transform) ) {
        return false;
    }
    
    for ( u32 meshNum=0; meshNum<geometry->m_meshNodes.size(); meshNum++ )
    {
        Mesh* curMesh = geometry->m_meshNodes[meshNum].mesh;

        if (curMesh == mesh) {
          drawMeshNode( geometry, meshNum, fn );
          return true;
        }
    }
    
    LOG_E("Renderer", "renderGeometryMesh: Mesh not found!");
    return false;
}


bool GLES20_Renderer::endScene()
{
    return true;
}


void GLES20_Renderer::pushState ()
{
}

void GLES20_Renderer::popState ()
{
}

