#include <jni.h>
#include "slackgine.h"
#include "stl_jni_bind.h"

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT jlong    JNICALL   Java_es_lautech_slackgine_l3m_Createl3mInstance(JNIEnv*, jobject);
    JNIEXPORT jlong    JNICALL   Java_es_lautech_slackgine_l3m_Createl3mInstance_1type(JNIEnv*, jobject, jstring);
    JNIEXPORT void     JNICALL   Java_es_lautech_slackgine_l3m_Destroyl3mInstance(JNIEnv*, jobject, jlong);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_Load(JNIEnv*, jobject, jobject); 
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_Save(JNIEnv*, jobject, jobject);
    JNIEXPORT jstring  JNICALL   Java_es_lautech_slackgine_l3m_type(JNIEnv*, jobject );
    JNIEXPORT jstring  JNICALL   Java_es_lautech_slackgine_l3m_error(JNIEnv*, jobject );
    JNIEXPORT void     JNICALL   Java_es_lautech_slackgine_l3m_DeclareMetadata(JNIEnv*, jobject, jstring);
    
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_Write16(JNIEnv*, jobject, jshortArray, jint, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_Write32(JNIEnv*, jobject, jintArray, jint, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_Write64(JNIEnv*, jobject, jlongArray, jint, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_WriteFloat(JNIEnv*, jobject, jfloatArray, jint, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_WriteStr(JNIEnv*, jobject, jstring, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_WriteData(JNIEnv*, jobject, jbyteArray, jint, jint, jobject);
    
    JNIEXPORT jint     JNICALL   Java_es_lautech_slackgine_l3m_Read16(JNIEnv*, jobject, jshortArray, jint, jobject);
    JNIEXPORT jint     JNICALL   Java_es_lautech_slackgine_l3m_Read32(JNIEnv*, jobject, jintArray, jint, jobject);
    JNIEXPORT jint     JNICALL   Java_es_lautech_slackgine_l3m_Read64(JNIEnv*, jobject, jlongArray, jint, jobject);
    JNIEXPORT jint     JNICALL   Java_es_lautech_slackgine_l3m_ReadFloat(JNIEnv*, jobject, jfloatArray, jint, jobject);
    JNIEXPORT jstring  JNICALL   Java_es_lautech_slackgine_l3m_ReadStr(JNIEnv*, jobject, jobject);
    JNIEXPORT jint     JNICALL   Java_es_lautech_slackgine_l3m_ReadData(JNIEnv*, jobject, jbyteArray, jint, jint, jobject);
}
#endif

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



static javal3m* Getl3m ( JNIEnv* env, jobject thiz )
{
    jclass l3mClass = env->FindClass ( "es.lautech.slackgine.l3m" );
    jfieldID instanceFieldID = env->GetFieldID ( l3mClass, "m_jniInstance", "J" );
    return reinterpret_cast < javal3m *> ( env->GetLongField ( thiz, instanceFieldID ) );
}

JNIEXPORT jlong JNICALL
Java_es_lautech_slackgine_l3m_Createl3mInstance ( JNIEnv* env, jobject thiz )
{
    javal3m* instance = new javal3m (env, thiz);
    return reinterpret_cast<long> ( instance );
}

JNIEXPORT jlong JNICALL
Java_es_lautech_slackgine_l3m_Createl3mInstance_1type ( JNIEnv* env, jobject thiz, jstring type )
{
    const char* typestr = env->GetStringUTFChars ( type, 0 );
    javal3m* instance = new javal3m ( env, thiz, typestr );
    env->ReleaseStringUTFChars ( type, typestr );
    return reinterpret_cast<long> ( instance );
}

JNIEXPORT void JNICALL
Java_es_lautech_slackgine_l3m_Destroyl3mInstance ( JNIEnv* env, jobject thiz, jlong instance_ )
{
    javal3m* instance = reinterpret_cast<javal3m *>(instance_);
    delete instance;
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_Load ( JNIEnv* env, jobject thiz, jobject is )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_istream<char, false> stream ( env, is );
    return ( instance->Load ( stream ) == l3m::OK );
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_Save ( JNIEnv* env, jobject thiz, jobject os )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_ostream<char, false> stream ( env, os );
    return ( instance->Save ( stream ) == l3m::OK );
}

JNIEXPORT jstring JNICALL
Java_es_lautech_slackgine_l3m_type ( JNIEnv* env, jobject thiz )
{
    javal3m* instance = Getl3m ( env, thiz );
    return env->NewStringUTF ( instance->type().c_str() );
}

JNIEXPORT jstring JNICALL
Java_es_lautech_slackgine_l3m_error ( JNIEnv* env, jobject thiz )
{
    javal3m* instance = Getl3m ( env, thiz );
    return env->NewStringUTF ( instance->error() );
}

JNIEXPORT void JNICALL
Java_es_lautech_slackgine_l3m_DeclareMetadata ( JNIEnv* env, jobject thiz, jstring name )
{
    javal3m* instance = Getl3m ( env, thiz );
    const char* chars = env->GetStringUTFChars(name, 0);
    instance->DeclareMetadata ( chars );
    env->ReleaseStringUTFChars(name, chars);
}


JNIEXPORT jint JNICALL
Java_es_lautech_slackgine_l3m_Read16 ( JNIEnv* env, jobject thiz, jshortArray array, jint nmemb, jobject is )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_istream<char, false> stream ( env, is );
    
    jshort* elems = env->GetShortArrayElements(array, 0);
    jint bytes = instance->Read16((u16*)elems, nmemb, stream);
    env->ReleaseShortArrayElements(array, elems, 0);
    
    return bytes;
}

JNIEXPORT jint JNICALL
Java_es_lautech_slackgine_l3m_Read32 ( JNIEnv* env, jobject thiz, jintArray array, jint nmemb, jobject is )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_istream<char, false> stream ( env, is );
    
    jint* elems = env->GetIntArrayElements(array, 0);
    jint bytes = instance->Read32((u32*)elems, nmemb, stream);
    env->ReleaseIntArrayElements(array, elems, 0);
    
    return bytes;
}

JNIEXPORT jint JNICALL
Java_es_lautech_slackgine_l3m_Read64 ( JNIEnv* env, jobject thiz, jlongArray array, jint nmemb, jobject is )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_istream<char, false> stream ( env, is );
    
    jlong* elems = env->GetLongArrayElements(array, 0);
    jint bytes = instance->Read64((u64*)elems, nmemb, stream);
    env->ReleaseLongArrayElements(array, elems, 0);
    
    return bytes;
}

JNIEXPORT jint JNICALL
Java_es_lautech_slackgine_l3m_ReadFloat ( JNIEnv* env, jobject thiz, jfloatArray array, jint nmemb, jobject is )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_istream<char, false> stream ( env, is );
    
    jfloat* elems = env->GetFloatArrayElements(array, 0);
    jint bytes = instance->ReadFloat((float*)elems, nmemb, stream);
    env->ReleaseFloatArrayElements(array, elems, 0);
    
    return bytes;
}

JNIEXPORT jstring JNICALL
Java_es_lautech_slackgine_l3m_ReadStr ( JNIEnv* env, jobject thiz, jobject is )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_istream<char, false> stream ( env, is );
    
    std::string temp;
    instance->ReadStr(temp, stream);
    return env->NewStringUTF(temp.c_str());
}

JNIEXPORT jint JNICALL
Java_es_lautech_slackgine_l3m_ReadData ( JNIEnv* env, jobject thiz, jbyteArray array, jint size, jint nmemb, jobject is )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_istream<char, false> stream ( env, is );
    
    jbyte* elems = env->GetByteArrayElements(array, 0);
    jint bytes = instance->ReadData((char*)elems, size, nmemb, stream);
    env->ReleaseByteArrayElements(array, elems, 0);
    
    return bytes;
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_Write16 ( JNIEnv* env, jobject thiz, jshortArray array, jint nmemb, jobject os )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_ostream<char, false> stream ( env, os );
    
    jboolean isCopy;
    jshort* elems = env->GetShortArrayElements(array, &isCopy);
    bool state = instance->Write16((const u16*)elems, nmemb, stream);
    env->ReleaseShortArrayElements(array, elems, isCopy ? JNI_ABORT : 0);
    
    return state;
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_Write32 ( JNIEnv* env, jobject thiz, jintArray array, jint nmemb, jobject os )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_ostream<char, false> stream ( env, os );
    
    jboolean isCopy;
    jint* elems = env->GetIntArrayElements(array, &isCopy);
    bool state = instance->Write32((const u32*)elems, nmemb, stream);
    env->ReleaseIntArrayElements(array, elems, isCopy ? JNI_ABORT : 0);
    
    return state;
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_Write64 ( JNIEnv* env, jobject thiz, jlongArray array, jint nmemb, jobject os )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_ostream<char, false> stream ( env, os );
    
    jboolean isCopy;
    jlong* elems = env->GetLongArrayElements(array, &isCopy);
    bool state = instance->Write64((const u64*)elems, nmemb, stream);
    env->ReleaseLongArrayElements(array, elems, isCopy ? JNI_ABORT : 0);
    
    return state;
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_WriteStr ( JNIEnv* env, jobject thiz, jstring str, jobject os )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_ostream<char, false> stream ( env, os );
    
    const char* elems = env->GetStringUTFChars(str, 0);
    bool state = instance->WriteStr((const char*)elems, stream);
    env->ReleaseStringUTFChars(str, elems);
    
    return true;
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_WriteData ( JNIEnv* env, jobject thiz, jbyteArray array, jint size, jint nmemb, jobject os )
{
    javal3m* instance = Getl3m ( env, thiz );
    jni_ostream<char, false> stream ( env, os );
    
    jboolean isCopy;
    jbyte* elems = env->GetByteArrayElements(array, &isCopy);
    bool state = instance->WriteData((const char*)elems, size, nmemb, stream);
    env->ReleaseByteArrayElements(array, elems, isCopy ? JNI_ABORT : 0);
    
    return state;
}
