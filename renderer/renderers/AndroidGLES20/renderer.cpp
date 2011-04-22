#include "gles20.h"

struct RendererData : public l3m::IRendererData
{
    GLsizei         m_numbuffers;
    GLuint*         m_buffers;

    RendererData () : m_numbuffers(0), m_buffers(0) {}

    ~RendererData ()
    {
        if ( m_buffers != 0 )
        {
            glDeleteBuffers ( m_numbuffers, m_buffers );
            eglGetError();
            delete [] m_buffers;
        }
    }
};

GLES20_Renderer::GLES20_Renderer()
: m_vertexShader(0)
, m_fragmentShader(0)
, m_program(0)
{
}

GLES20_Renderer::~GLES20_Renderer()
{
    if ( m_program != 0 )
    {
        delete m_vertexShader;
        delete m_fragmentShader;
        delete m_program;
    }
}

bool GLES20_Renderer::Initialize()
{
    // Initialize the main shaders
    static const char* const s_defaultVertexShader =
        "in vec3 in_Position;\n"
        "in vec3 in_Normal;\n"
        "in vec2 in_Tex2D;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "       gl_Position = vec4(in_Position, 1.0);\n"
        "}\n";

    static const char* const s_defaultFragmentShader =
        "void main(void)\n"
        "{\n"
        "       gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
        "}\n";

    std::istringstream vertexShaderSource ( s_defaultVertexShader );
    std::istringstream fragmentShaderSource ( s_defaultFragmentShader );

    m_program = new GLES20_Program ();
    m_vertexShader = new GLES20_Shader (IShader::VERTEX_SHADER);
    m_fragmentShader = new GLES20_Shader (IShader::FRAGMENT_SHADER);

    if ( !m_vertexShader->Load (vertexShaderSource) || !m_fragmentShader->Load ( fragmentShaderSource ) )
        return false;
    if ( !m_program->AttachShader(m_vertexShader) || !m_program->AttachShader(m_fragmentShader) )
        return false;
    if ( !m_program->Link() )
        return false;

    return true;
}

bool GLES20_Renderer::SetupModel(const l3m* model)
{
    if ( model->rendererData() != 0 )
        return false;

    RendererData* data = new RendererData ();
    model->rendererData() = data;

    // Count the number of meshes.
    unsigned int numMeshes = 0;
    const l3m::groupMap& groups = model->GetGroups();
    for ( l3m::groupMap::const_iterator i = groups.begin(); i != groups.end(); ++i )
    {
        const l3m::meshList& meshes = i->second;
        numMeshes += meshes.size();
    }

    if ( numMeshes > 0 )
    {
        // Generate the buffers
        data->m_numbuffers = numMeshes * 2;

        data->m_buffers = new GLuint [ data->m_numbuffers ];
        glGenBuffers ( data->m_numbuffers, &(data->m_buffers[0]) );
        eglGetError();

        // Setup the vaos
        unsigned int curMesh = 0;
        for ( l3m::groupMap::const_iterator i = groups.begin(); i != groups.end(); ++i )
        {
            const l3m::meshList& meshes = i->second;
            for ( l3m::meshList::const_iterator j = meshes.begin(); j != meshes.end(); ++j )
            {
                Mesh* mesh = *j;
                glBindBuffer ( GL_ARRAY_BUFFER, data->m_buffers[curMesh * 2] );
                eglGetError();
                glBufferData ( GL_ARRAY_BUFFER, mesh->numVertices() * sizeof(Vertex), mesh->vertices(), GL_STATIC_DRAW );
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

bool GLES20_Renderer::BeginScene ()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    eglGetError();

    if ( !m_program->Use () )
        return false;

    return true;
}

bool GLES20_Renderer::RenderEntity ( const Entity* entity )
{
    const l3m* model = entity->GetModel ();
    if ( model )
    {
        l3m::IRendererData* data_ = model->rendererData();
        if ( !data_ && !SetupModel(model) )
            return false;
        RendererData* data = static_cast<RendererData*>(model->rendererData ());

        unsigned int curMesh = 0;
        const l3m::groupMap& groups = model->GetGroups();
        for ( l3m::groupMap::const_iterator i = groups.begin(); i != groups.end(); ++i )
        {
            const l3m::meshList& meshes = i->second;
            for ( l3m::meshList::const_iterator j = meshes.begin(); j != meshes.end(); ++j )
            {
                const Mesh* mesh = *j;

                GLenum polyType = GL_INVALID_ENUM;
                switch ( mesh->polyType() )
                {
                    case Mesh::TRIANGLES: polyType = GL_TRIANGLES; break;
                    case Mesh::TRIANGLE_STRIP: polyType = GL_TRIANGLE_STRIP; break;
                    case Mesh::TRIANGLE_FAN: polyType = GL_TRIANGLE_FAN; break;
                    default: break;
                }

                if ( polyType != GL_INVALID_ENUM )
                {
                    glBindBuffer ( GL_ARRAY_BUFFER, data->m_buffers[curMesh * 2] );
                    eglGetError();
                    glVertexAttribPointer ( GLES20_Program::POSITION, 3, GL_FLOAT, GL_FALSE, 20, (void*)0 );
                    eglGetError();
                    glVertexAttribPointer ( GLES20_Program::NORMAL, 3, GL_FLOAT, GL_TRUE, 20, (void*)12 );
                    eglGetError();
                    glVertexAttribPointer ( GLES20_Program::TEX2D, 2, GL_FLOAT, GL_TRUE, 24, (void*)24 );
                    eglGetError();
                    glEnableVertexAttribArray ( GLES20_Program::POSITION );
                    eglGetError();
                    glEnableVertexAttribArray ( GLES20_Program::NORMAL );
                    eglGetError();
                    glEnableVertexAttribArray ( GLES20_Program::TEX2D );
                    eglGetError();

                    glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, data->m_buffers[curMesh * 2 + 1 ] );
                    eglGetError();

                    glDrawElements ( polyType, mesh->numIndices(), GL_UNSIGNED_INT, 0 );
                    eglGetError();
                }

                ++curMesh;
            }
        }
    }
    return true;
}

bool GLES20_Renderer::EndScene()
{
    return true;
}
