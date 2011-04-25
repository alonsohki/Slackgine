#include <jni.h>
#include "slackgine.h"

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT jint JNICALL   Java_es_lautech_slackgine_STListream_read(JNIEnv*, jobject, jbyteArray, jint, jint);
    JNIEXPORT void JNICALL   Java_es_lautech_slackgine_STLostream_write(JNIEnv*, jobject, jbyteArray, jint, jint);
}
#endif

jobject CreateSTListream ( JNIEnv* env, std::istream* istream )
{
    jclass STListreamClass = env->FindClass ( "es.lautech.slackgine.STListream" );
    jmethodID InitID = env->GetMethodID ( STListreamClass, "<init>", "()V" );
    jobject instance = env->NewObject ( STListreamClass, InitID );
        
    jfieldID instanceFieldID = env->GetFieldID ( STListreamClass, "m_jniInstance", "J" );
    env->SetLongField ( instance, instanceFieldID, reinterpret_cast<jlong>(istream) );
    
    return instance;
}

jobject CreateSTLostream ( JNIEnv* env, std::ostream* ostream )
{
    jclass STLostreamClass = env->FindClass ( "es.lautech.slackgine.STLostream" );
    jmethodID InitID = env->GetMethodID ( STLostreamClass, "<init>", "()V" );
    jobject instance = env->NewObject ( STLostreamClass, InitID );
        
    jfieldID instanceFieldID = env->GetFieldID ( STLostreamClass, "m_jniInstance", "J" );
    env->SetLongField ( instance, instanceFieldID, reinterpret_cast<jlong>(ostream) );
    
    return instance;
}


static std::istream* Getistream ( JNIEnv* env, jobject thiz )
{
    jclass STListreamClass = env->FindClass ( "es.lautech.slackgine.STListream" );
    jfieldID instanceFieldID = env->GetFieldID ( STListreamClass, "m_jniInstance", "J" );
    return reinterpret_cast < std::istream *> ( env->GetLongField ( thiz, instanceFieldID ) );
}

static std::ostream* Getostream ( JNIEnv* env, jobject thiz )
{
    jclass STLostreamClass = env->FindClass ( "es.lautech.slackgine.STLostream" );
    jfieldID instanceFieldID = env->GetFieldID ( STLostreamClass, "m_jniInstance", "J" );
    return reinterpret_cast < std::ostream *> ( env->GetLongField ( thiz, instanceFieldID ) );
}

JNIEXPORT jint JNICALL
Java_es_lautech_slackgine_STListream_read(JNIEnv* env, jobject thiz, jbyteArray array, jint start, jint count)
{
    
}

JNIEXPORT void JNICALL
Java_es_lautech_slackgine_STListream_write(JNIEnv* env, jobject thiz, jbyteArray array, jint start, jint count)
{
    
}