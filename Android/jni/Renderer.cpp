#include <jni.h>
#include "slackgine.h"
#include "javal3m.h"

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_Renderer_Initialize(JNIEnv*, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_Renderer_BeginScene(JNIEnv*, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_Renderer_Render(JNIEnv*, jobject, jobject);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_Renderer_EndScene(JNIEnv*, jobject); 
}
#endif

extern javal3m* Getl3m ( JNIEnv* env, jobject thiz );
IRenderer* GetRenderer ( JNIEnv* env, jobject thiz )
{
    jclass RendererClass = env->FindClass("es.lautech.slackgine.Renderer");
    jfieldID fieldID = env->GetFieldID(RendererClass, "m_jniInstance", "J" );
    return reinterpret_cast<IRenderer* > ( env->GetLongField(thiz, fieldID) );
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_Renderer_Initialize(JNIEnv* env, jobject thiz)
{
    IRenderer* renderer = GetRenderer ( env, thiz );
    return renderer->Initialize ();
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_Renderer_BeginScene(JNIEnv* env, jobject thiz)
{
    IRenderer* renderer = GetRenderer ( env, thiz );
    return renderer->BeginScene ();
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_Renderer_Render(JNIEnv* env, jobject thiz, jobject model)
{
    IRenderer* renderer = GetRenderer ( env, thiz );
    javal3m* jmodel = Getl3m ( env, model );
    return renderer->Render ( jmodel );
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_Renderer_EndScene(JNIEnv* env, jobject thiz)
{
    IRenderer* renderer = GetRenderer ( env, thiz );
    return renderer->EndScene ();
}
