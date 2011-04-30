#include <sstream>
#include "opengl3.h"

struct RendererData : public l3m::IRendererData
{
    GLsizei         m_numvaos;
    GLuint*         m_vaos;
    GLsizei         m_numbuffers;
    GLuint*         m_buffers;

    RendererData () : m_numvaos(0), m_vaos(0), m_numbuffers(0), m_buffers(0) {}

    ~RendererData ()
    {
        if ( m_vaos != 0 )
        {
            glDeleteVertexArrays ( m_numvaos, m_vaos );
            eglGetError();
            delete [] m_vaos;
        }
        if ( m_buffers != 0 )
        {
            glDeleteBuffers ( m_numbuffers, m_buffers );
            eglGetError();
            delete [] m_buffers;
        }
    }
};

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
        "void main(void)\n"
        "{\n"
        " float s = 10;\n"
        " float l = -s; float r = s; float b = -s; float t = s; float n = s; float f = -s;\n"
        " mat4 ortho = mat4(vec4( 2/(r-l),     0,            0,            0 ),"
                           "vec4( 0,           2/(t-b),      0,            0 ),"
                           "vec4( 0,           0,            (-2)/(f-n),   0 ),"
                           "vec4(-(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1));\n"
        " mat4 rot1 = mat4(vec4(cos(-1), sin(-1), 0, 0), vec4(-sin(-1), cos(-1), 0, 0), vec4(0, 0,1,0), vec4(0,0,0,1));\n"
        " mat4 rot2 = mat4(vec4(cos(-1), 0, -sin(-1), 0), vec4(0, 1, 0, 0), vec4(sin(-1),0,cos(-1),0), vec4(0,0,0,1));\n"
        "       gl_Position = vec4(in_Position, 1.0) * ortho;\n"
        "}\n";

    static const char* const s_defaultFragmentShader =
        "void main(void)\n"
        "{\n"
        " vec3 rgb = vec3(gl_FragCoord.xy, gl_FragCoord.z * 1000);\n"
        "       gl_FragColor = vec4(normalize(rgb), 1);\n"
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

bool OpenGL3_Renderer::SetupModel(const l3m* model)
{
    if ( model->rendererData() != 0 )
        return false;

    RendererData* data = new RendererData ();
    model->rendererData() = data;

    // Count the number of meshes.
    unsigned int numMeshes = 0;
    const l3m::geometryList& geometries = model->geometries();
    for ( l3m::geometryList::const_iterator i = geometries.begin(); i != geometries.end(); ++i )
    {
        const Geometry* geometry = *i;
        numMeshes += geometry->meshes().size();
    }

    if ( numMeshes > 0 )
    {
        // Generate the buffers
        data->m_numvaos = numMeshes;
        data->m_numbuffers = numMeshes * 2;

        data->m_vaos = new GLuint [ data->m_numvaos ];
        glGenVertexArrays ( data->m_numvaos, &(data->m_vaos[0]) );
        eglGetError();

        data->m_buffers = new GLuint [ data->m_numbuffers ];
        glGenBuffers ( data->m_numbuffers, &(data->m_buffers[0]) );
        eglGetError();

        // Setup the vaos
        unsigned int curMesh = 0;
        for ( l3m::geometryList::const_iterator i = geometries.begin(); i != geometries.end(); ++i )
        {
            const Geometry* geometry = *i;
            for ( Geometry::meshList::const_iterator j = geometry->meshes().begin(); j != geometry->meshes().end(); ++j )
            {
                Mesh* mesh = *j;
                
                glBindVertexArray ( data->m_vaos[curMesh] );
                eglGetError();

                glBindBuffer ( GL_ARRAY_BUFFER, data->m_buffers[curMesh * 2] );
                eglGetError();
                glBufferData ( GL_ARRAY_BUFFER, mesh->numVertices() * sizeof(Vertex), mesh->vertices(), GL_STATIC_DRAW );
                eglGetError();
                glVertexAttribPointer ( OpenGL3_Program::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLchar*)0 );
                eglGetError();
                glVertexAttribPointer ( OpenGL3_Program::NORMAL, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLchar*)12 );
                eglGetError();
                glVertexAttribPointer ( OpenGL3_Program::TEX2D, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLchar*)24 );
                eglGetError();
                glEnableVertexAttribArray ( OpenGL3_Program::POSITION );
                eglGetError();
                glEnableVertexAttribArray ( OpenGL3_Program::NORMAL );
                eglGetError();
                glEnableVertexAttribArray ( OpenGL3_Program::TEX2D );
                eglGetError();

                glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, data->m_buffers[curMesh * 2 + 1 ] );
                eglGetError();
                glBufferData ( GL_ELEMENT_ARRAY_BUFFER, mesh->numIndices() * sizeof(unsigned int), mesh->indices(), GL_STATIC_DRAW );
                eglGetError();

                ++curMesh;
            }
        }
    }

    return true;
}

bool OpenGL3_Renderer::BeginScene ()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    eglGetError();

    if ( !m_program->Use () )
        return false;
    
    glEnable ( GL_DEPTH_TEST );
    glCullFace ( GL_BACK );

    return true;
}

bool OpenGL3_Renderer::Render ( const l3m* model )
{
    l3m::IRendererData* data_ = model->rendererData();
    if ( !data_ && !SetupModel(model) )
        return false;
    RendererData* data = static_cast<RendererData*>(model->rendererData ());

    unsigned int curMesh = 0;
    const l3m::geometryList& geometries = model->geometries();
    for ( l3m::geometryList::const_iterator i = geometries.begin(); i != geometries.end(); ++i )
    {
        const Geometry* geometry = *i;
        
        const Geometry::meshList& meshes = geometry->meshes();;
        for ( Geometry::meshList::const_iterator j = meshes.begin(); j != meshes.end(); ++j, ++curMesh )
        {
            const Mesh* mesh = *j;
            
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
                glBindVertexArray ( data->m_vaos[curMesh] );
                eglGetError();
                glDrawElements ( GL_TRIANGLES, mesh->numIndices(), GL_UNSIGNED_INT, 0 );
                eglGetError();
            }
        }
    }
    
    return true;
}

bool OpenGL3_Renderer::EndScene()
{
    return true;
}
