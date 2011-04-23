#include "opengl3.h"

IRenderer* Renderer::CreateRenderer()
{
    return new OpenGL3_Renderer ();
}

IProgram* Renderer::CreateProgram()
{
    return new OpenGL3_Program ();
}

IShader* Renderer::CreateShader(IShader::Type type, const char* file)
{
    IShader* shader = new OpenGL3_Shader ( type );
    shader->Load ( file );
    return shader;
}

IShader* Renderer::CreateShader(IShader::Type type, std::istream& fp)
{
    IShader* shader = new OpenGL3_Shader ( type );
    shader->Load ( fp );
    return shader;
}
