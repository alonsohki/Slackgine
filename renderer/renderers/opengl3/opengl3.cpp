#include "opengl3.h"

using namespace Renderer;

IRenderer* Factory::CreateRenderer()
{
    return new OpenGL3_Renderer ();
}

IProgram* Factory::CreateProgram()
{
    return new OpenGL3_Program ();
}

IShader* Factory::CreateShader(IShader::Type type, const char* file)
{
    IShader* shader = new OpenGL3_Shader ( type );
    shader->Load ( file );
    return shader;
}

IShader* Factory::CreateShader(IShader::Type type, std::istream& fp)
{
    IShader* shader = new OpenGL3_Shader ( type );
    shader->Load ( fp );
    return shader;
}
