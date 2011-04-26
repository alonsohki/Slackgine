#ifndef JAVAL3M_H
#define	JAVAL3M_H

#include <jni.h>
#include "l3m/l3m.h"

extern jobject CreateSTListream ( JNIEnv*, std::istream* );
extern jobject CreateSTLostream ( JNIEnv*, std::ostream* );

class javal3m : public l3m
{
private:
    JNIEnv*     m_env;
    jobject     m_object;
    jmethodID   m_loadMethodID;
    jmethodID   m_saveMethodID;
    
public:
                javal3m                 ( JNIEnv* env, jobject object ) : l3m (), m_env(env), m_object(object) { Initialize(); }
                javal3m                 ( JNIEnv* env, jobject object, const char* type ) : l3m ( type ), m_env(env), m_object(object) { Initialize(); }
                ~javal3m                () {}
                
private:
    void        Initialize              ()
    {
        jclass clazz = m_env->GetObjectClass(m_object);
        m_loadMethodID = m_env->GetMethodID(clazz, "LoadMetadata", "(Ljava/lang/String;Ljava/io/InputStream;)Z" );
        m_saveMethodID = m_env->GetMethodID(clazz, "SaveMetadata", "(Ljava/lang/String;Ljava/io/OutputStream;)Z" );
    }
    
public:    
    void        DeclareMetadata         ( const char* name ) { l3m::DeclareMetadata ( name ); }
    bool        SaveMetadata            ( const std::string& name, std::ostream& fp )
    {
        jobject stream = CreateSTLostream ( m_env, &fp );
        jstring strName = m_env->NewStringUTF(name.c_str());
        return m_env->CallBooleanMethod(m_object, m_saveMethodID, strName, stream);
    }
    bool        LoadMetadata            ( const std::string& name, std::istream& fp )
    {
        jobject stream = CreateSTListream ( m_env, &fp );
        jstring strName = m_env->NewStringUTF(name.c_str());
        return m_env->CallBooleanMethod(m_object, m_loadMethodID, strName, stream);
    }
    
    bool        Write16                 ( const u16* v, size_t nmemb, std::ostream& fp ) { return l3m::Write16(v, nmemb, fp ); }
    bool        Write32                 ( const u32* v, size_t nmemb, std::ostream& fp ) { return l3m::Write32(v, nmemb, fp ); }
    bool        Write64                 ( const u64* v, size_t nmemb, std::ostream& fp ) { return l3m::Write64(v, nmemb, fp ); }
    bool        WriteFloat              ( const float* v, size_t nmemb, std::ostream& fp ) { return l3m::WriteFloat(v, nmemb, fp ); }
    bool        WriteStr                ( const std::string& str, std::ostream& fp ) { return l3m::WriteStr(str, fp ); }
    bool        WriteData               ( const char* v, size_t size, size_t nmemb, std::ostream& fp ) { return l3m::WriteData(v, size, nmemb, fp ); }
    
    size_t      Read16                  ( u16* v, size_t nmemb, std::istream& fp ) { return l3m::Read16(v, nmemb, fp); }
    size_t      Read32                  ( u32* v, size_t nmemb, std::istream& fp ) { return l3m::Read32(v, nmemb, fp); }
    size_t      Read64                  ( u64* v, size_t nmemb, std::istream& fp ) { return l3m::Read64(v, nmemb, fp); }
    size_t      ReadFloat               ( float* v, size_t nmemb, std::istream& fp ) { return l3m::ReadFloat(v, nmemb, fp); }
    size_t      ReadStr                 ( std::string& dest, std::istream& fp ) { return l3m::ReadStr(dest, fp); }
    size_t      ReadData                ( char* v, size_t size, size_t nmemb, std::istream& fp ) { return l3m::ReadData(v, size, nmemb, fp); }
};


#endif	/* JAVAL3M_H */

