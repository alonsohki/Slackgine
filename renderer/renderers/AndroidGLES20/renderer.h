#ifndef GLES20_RENDERER_H
#define	GLES20_RENDERER_H

#include <sstream>
#include "gles20.h"

namespace Renderer
{
    
class GLES20_Renderer : public IRenderer
{
private:
    bool                m_initialized;
    IShader*            m_vertexShader;
    IShader*            m_fragmentShader;
    IProgram*           m_program;
    char                m_error [ 512 ];
    Matrix              m_matProjection;
    Matrix              m_matLookat;
    Vector3             m_viewVector;
    TextureLookupFn     m_texLookup;

public:
                GLES20_Renderer        ();
                ~GLES20_Renderer       ();
    
    bool        initialize              ();

    bool        beginScene              ( const Matrix& projection, const Matrix& lookAt, TextureLookupFn fn );
    void        setProgram              ( IProgram* program );
    bool        drawMeshNode            ( Geometry* geometry, u32 meshNum, MeshRenderFn fn );
    bool        prepareGeometry         ( Geometry* geometry, const Transform& transform );
    bool        render                  ( Geometry* geometry, const Transform& transform, bool includeTransparent, MeshRenderFn fn = 0 );
    bool        renderGeometryMesh      ( Geometry* geometry, Mesh* mesh, const Transform& transform, MeshRenderFn fn );
    bool        endScene                ();

    void        pushState               ();
    void        popState                ();

    void        getError                ( char* dest ) const { strcpy(dest, m_error); }
        
private:
    void        setupLighting           ();
};

};

#endif	/* GLES20_RENDERER_H */

