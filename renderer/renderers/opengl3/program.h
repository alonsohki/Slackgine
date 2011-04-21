#ifndef OPENGL3_PROGRAM_H
#define	OPENGL3_PROGRAM_H

#include "../../program.h"

class OpenGL3_Program : public IProgram
{
private:
    GLuint      m_handler;
    bool        m_linked;
    
public:
    OpenGL3_Program ()
    : m_linked ( false )
    {
        m_handler = glCreateProgram ();
    }
    
    ~OpenGL3_Program ()
    {
        if ( m_handler > 0 )
            glDeleteProgram ( m_handler );
    }
    
    bool AttachShader ( const IShader* shader_ )
    {
        const OpenGL3_Shader* shader = static_cast<const OpenGL3_Shader*>(shader_);
        if ( m_handler == 0 || shader->Ok() == false )
            return false;
        
        glAttachShader ( m_handler, shader->handler() );
        return true;
    }
    
    bool DetachShader ( const IShader* shader_ )
    {
        const OpenGL3_Shader* shader = static_cast<const OpenGL3_Shader*>(shader_);
        if ( m_handler == 0 || shader->Ok() == false )
            return false;
        
        glDetachShader ( m_handler, shader->handler() );
        return true;
    }
    
    bool Link ()
    {
        if ( m_handler == 0 )
            return false;
        
        glLinkProgram ( m_handler );
        
        GLint linked;
        glGetProgramiv ( m_handler, GL_LINK_STATUS, &linked );
        if ( linked == GL_TRUE )
            m_linked = true;
        else
            m_linked = false;
        return m_linked;
    }
    
    bool Use ()
    {
        if ( Ok() )
            glUseProgram ( m_handler );
        return Ok ();
    }
    
    bool Ok () const
    {
        return m_linked && m_handler > 0;
    }
    
    GLuint&             handler () { return m_handler; }
    const GLuint&       handler () const { return m_handler; }
};

#endif	/* OPENGL3_PROGRAM_H */

