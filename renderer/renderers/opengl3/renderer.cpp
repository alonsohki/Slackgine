#include <sstream>
#include "opengl3.h"

using namespace Renderer;

OpenGL3_Renderer::OpenGL3_Renderer()
: m_initialized(false)
, m_vertexShader(0)
, m_fragmentShader(0)
, m_program(0)
{
    strcpy ( m_error, "Success" );
}

OpenGL3_Renderer::~OpenGL3_Renderer()
{
    if ( m_program != 0 )
    {
        delete m_vertexShader;
        delete m_fragmentShader;
        delete m_program;
    }
}

bool OpenGL3_Renderer::Initialize()
{
    if ( m_initialized )
        return true;

    // Initialize GLEW
    if ( glewInit () != 0 )
            return false;

    // Initialize the main shaders
    static const char* const s_defaultVertexShader =
        "in vec3 in_Position;\n"
        "in vec3 in_Normal;\n"
        "in vec2 in_Tex2D;\n"
        "\n"
        "uniform mat4 un_Matrix;\n"
        "uniform mat4 un_ProjectionMatrix;\n"
        "uniform mat4 un_LookatMatrix;\n"
        "uniform mat4 un_ModelviewMatrix;\n"
        "\n"
        "uniform mat4 un_NormalMatrix;\n"
        "\n"
        "varying vec3 ex_Normal;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = vec4(in_Position, 1.0) * un_Matrix;\n"
        "    ex_Normal = vec4(in_Normal, 1.0) * un_NormalMatrix;\n"
        "}\n";

    static const char* const s_defaultFragmentShader =
        "varying vec3 ex_Normal;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        " ex_Normal = ex_Normal + vec4(0.15, 0.15, 0.15, 0);\n"
        "    gl_FragColor = vec4(ex_Normal, 1.0);\n"
        "}\n";

    std::istringstream vertexShaderSource ( s_defaultVertexShader );
    std::istringstream fragmentShaderSource ( s_defaultFragmentShader );

    m_program = new OpenGL3_Program ();
    m_vertexShader = new OpenGL3_Shader (IShader::VERTEX_SHADER);
    m_fragmentShader = new OpenGL3_Shader (IShader::FRAGMENT_SHADER);

    strcpy ( m_error, "Success" );
    
    if ( !m_vertexShader->Load (vertexShaderSource) )
        m_vertexShader->GetError ( m_error );
    else if ( !m_fragmentShader->Load ( fragmentShaderSource ) )
        m_fragmentShader->GetError( m_error );
    else if ( !m_program->AttachShader(m_vertexShader) || !m_program->AttachShader(m_fragmentShader) )
        m_program->GetError ( m_error );
    else if ( !m_program->Link() )
        m_program->GetError( m_error );

    m_initialized = ( strcmp(m_error, "Success") == 0 );
    return m_initialized;
}

bool OpenGL3_Renderer::BeginScene ( const Matrix& matProjection, const Matrix& matLookat )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    eglGetError();

    if ( !m_program->Use () )
        return false;
    
    glEnable ( GL_DEPTH_TEST );
    glCullFace ( GL_BACK );

    m_matProjection = matProjection;
    m_matLookat = matLookat;
    
    return true;
}

bool OpenGL3_Renderer::Render ( Geometry* geometry, const Matrix& mat )
{
    if ( !geometry->initialized() )
        if ( !geometry->Initialize() )
            return false;

    Matrix matNormals = Matrix::Transpose(Matrix::Invert(mat));
    Matrix matGeometry = m_matProjection * m_matLookat * mat;
    
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

    // Bind the indices buffer
    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, geometry->m_elementBuffer );
    
    for ( Geometry::meshNodeVector::const_iterator iter = geometry->m_meshNodes.begin();
          iter != geometry->m_meshNodes.end();
          ++iter )
    {
        const Mesh* mesh = (*iter).mesh;

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
            m_program->SetUniform("un_ProjectionMatrix", m_matProjection );
            m_program->SetUniform("un_LookatMatrix", m_matLookat );
            m_program->SetUniform("un_ModelviewMatrix", mat);
            m_program->SetUniform("un_NormalMatrix", matNormals);
            m_program->SetUniform("un_Matrix", matGeometry );
            glDrawElements ( polyType, mesh->numIndices(), GL_UNSIGNED_INT, reinterpret_cast<const GLvoid *>((*iter).offset * sizeof(u32)) );
            eglGetError();
        }
    }
    
    return true;
}

bool OpenGL3_Renderer::EndScene()
{
    glutSwapBuffers ();
    return true;
}
