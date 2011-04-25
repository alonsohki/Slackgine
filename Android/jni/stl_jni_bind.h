#ifndef STL_JNI_BIND_H
#define	STL_JNI_BIND_H

#include <iostream>
#include <jni.h>

template < typename T = char, unsigned int N = 32 >
class jni_streambuffer : public std::basic_streambuf<T, std::char_traits<T> >
{
    T _inputBuffer [ N ];
    T _outputBuffer [ N ];
    typedef std::char_traits<T> _Tr;
    typedef std::basic_streambuf<T, std::char_traits<T> > _T_parent;
    JNIEnv* m_env;
    jobject m_istream;
    jmethodID m_inputMethod;
    jobject m_ostream;
    jmethodID m_outputMethod;
    
public:
    jni_streambuffer ( JNIEnv* env, jobject istream, jobject ostream )
    : m_env(env)
    , m_istream ( istream )
    , m_ostream ( ostream )
    {
        _T_parent::setp ( _outputBuffer, &_outputBuffer[N - 1] );
        _T_parent::setg ( _inputBuffer, _inputBuffer, &_inputBuffer[N - 1] );
        
        if ( m_istream )
        {
            jclass InputStreamClass = m_env->FindClass("java.io.InputStream");
            m_inputMethod = m_env->GetMethodID(InputStreamClass, "read", "([BII)I" );
        }
        
        if ( m_ostream )
        {
            jclass OutputStreamClass = m_env->FindClass("java.io.OutputStream");
            m_outputMethod = m_env->GetMethodID(OutputStreamClass, "write", "([BII)V" );
        }
    }
    
    virtual ~jni_streambuffer ()
    {
        sync ();
    }
    
protected:
    virtual int sync ()
    {
        if ( m_ostream && _T_parent::pptr() > _T_parent::pbase() )
        {
            WriteToJavaStream ( _T_parent::pbase(), (_T_parent::pptr () - _T_parent::pbase ()) * sizeof(T) );
        }
    }
    
    virtual int underflow ( int c )
    {
        if ( m_istream == 0 )
            return _Tr::eof();
        
        int size = ReadFromJavaStream ( _inputBuffer, sizeof(_inputBuffer) );
        if ( size < 0 )
            return _Tr::eof();

        _T_parent::setg ( _inputBuffer, _inputBuffer, &_inputBuffer[N - 1] );
        
        return _Tr::not_eof(_inputBuffer[0]);
    }
    
    virtual int overflow ( int c )
    {
        if ( m_ostream == 0 )
            return _Tr::eof();
        
        _outputBuffer[N-1] = _Tr::to_char_type(c);
        if ( WriteToJavaStream ( _outputBuffer, sizeof(_outputBuffer) ) < 0 )
            return _Tr::eof();
        
        _T_parent::setp ( _outputBuffer, &_outputBuffer[N - 1] );
        return _Tr::not_eof(c);
    }
    
private:
    int ReadFromJavaStream ( T* data, int size )
    {
        jbyteArray array = m_env->NewByteArray(size);
        int rdbytes = m_env->CallIntMethod(m_istream, m_inputMethod, array, 0, size );
        
        if ( rdbytes > 0 )
        {        
            jbyte* bytes = m_env->GetByteArrayElements ( array, 0 );
            memcpy ( data, bytes, rdbytes );
            m_env->ReleaseByteArrayElements ( array, bytes, JNI_ABORT );
        }
        
        return rdbytes;
    }
    
    int WriteToJavaStream ( const T* data, int size )
    {
        jbyteArray array = m_env->NewByteArray(size);
        jbyte* bytes = m_env->GetByteArrayElements ( array, 0 );
        memcpy ( bytes, data, size );
        m_env->ReleaseByteArrayElements ( array, bytes, 0 );
        
        int wrbytes = m_env->CallIntMethod(m_ostream, m_outputMethod, array, 0, size);
        return wrbytes;
    }
};

template < typename T = char >
class jni_istream : public std::basic_istream<T, std::char_traits<T> >
{
    jni_streambuffer<T> m_streambuffer;
    
public:
    jni_istream ( JNIEnv* env, jobject istream )
    : std::basic_istream<T, std::char_traits<T> > ( 0 )
    , m_streambuffer ( env, istream, 0 )
    {
        this->init(&m_streambuffer);
    }
};

template < typename T = char >
class jni_ostream : public std::basic_ostream<T, std::char_traits<T> >
{
    jni_streambuffer<T> m_streambuffer;
    
public:
    jni_ostream ( JNIEnv* env, jobject ostream )
    : std::basic_ostream<T, std::char_traits<T> > ( 0 )
    , m_streambuffer ( env, 0, ostream )
    {
        this->init(&m_streambuffer);
    }
};

template < typename T = char >
class jni_iostream : public std::basic_iostream<T, std::char_traits<T> >
{
    jni_streambuffer<T> m_streambuffer;
    
public:
    jni_iostream ( JNIEnv* env, jobject iostream )
    : std::basic_iostream<T, std::char_traits<T> > ( 0 )
    , m_streambuffer ( env, iostream, iostream )
    {
        this->init(&m_streambuffer);
    }
};


#endif	/* STL_JNI_BIND_H */
