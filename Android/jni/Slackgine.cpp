#include <jni.h>
#include "slackgine.h"

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT jlong JNICALL   Java_es_lautech_slackgine_Slackgine_CreateSlackgineInstance(JNIEnv*, jobject);
    JNIEXPORT void JNICALL    Java_es_lautech_slackgine_Slackgine_DestroySlackgineInstance(JNIEnv*, jobject, jlong instance);
    JNIEXPORT jobject JNICALL Java_es_lautech_slackgine_Slackgine_renderer(JNIEnv*, jobject);
}
#endif

static Slackgine* GetSlackgine ( JNIEnv* env, jobject thiz )
{
    jclass SlackgineClass = env->FindClass ( "es.lautech.slackgine.Slackgine" );
    jfieldID instanceFieldID = env->GetFieldID ( SlackgineClass, "m_jniInstance", "J" );
    return reinterpret_cast < Slackgine *> ( env->GetLongField ( thiz, instanceFieldID ) );
}

JNIEXPORT jlong JNICALL
Java_es_lautech_slackgine_Slackgine_CreateSlackgineInstance ( JNIEnv* env, jobject thiz )
{
    Slackgine* instance = new Slackgine ();
    return reinterpret_cast<long> ( instance );
}

JNIEXPORT void JNICALL
Java_es_lautech_slackgine_Slackgine_DestroySlackgineInstance ( JNIEnv* env, jobject thiz, jlong instance_ )
{
    Slackgine* instance = reinterpret_cast<Slackgine *>(instance_);
    delete instance;
}

JNIEXPORT jobject JNICALL
Java_es_lautech_slackgine_Slackgine_renderer ( JNIEnv* env, jobject thiz )
{
    Slackgine* sg = GetSlackgine ( env, thiz );
    jclass RendererClass = env->FindClass ( "es.lautech.slackgine.Renderer" );
    jmethodID InitID = env->GetMethodID ( RendererClass, "<init>", "()V" );
    jobject instance = env->NewObject ( RendererClass, InitID );
        
    jfieldID instanceFieldID = env->GetFieldID ( RendererClass, "m_jniInstance", "J" );
    env->SetLongField ( instance, instanceFieldID, reinterpret_cast<jlong>(sg->renderer()) );

    return instance;
}

