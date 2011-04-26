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
    // Obtenemos el stream
    std::istream* istream = Getistream ( env, thiz );
    
    // Reservamos espacio para leer los datos
    char* data = new char [ start + count ];
    istream->read ( data, start + count );
    
    // Comprobamos los bytes leídos en la última operación, descartando "start" bytes.
    int gcount = istream->gcount() - start;
    if ( gcount < 0 )
        gcount = 0;
    
    // Si hemos llegado a leer algo, lo copiamos al array de Java.
    if ( gcount > 0 )
    {
        jbyte* bytes = env->GetByteArrayElements(array, 0);
        memcpy ( bytes, &data[start], gcount);
        env->ReleaseByteArrayElements(array, bytes, 0);
    }
    
    delete [] data;
    return gcount;
}

JNIEXPORT void JNICALL
Java_es_lautech_slackgine_STLostream_write(JNIEnv* env, jobject thiz, jbyteArray array, jint start, jint count)
{
    // Obtenemos el stream
    std::ostream* ostream = Getostream ( env, thiz );
    
    // Escribimos
    jboolean isCopy;
    jbyte* bytes = env->GetByteArrayElements(array, &isCopy);
    ostream->write((const char*)&bytes[start], count);
    env->ReleaseByteArrayElements(array, bytes, isCopy ? JNI_ABORT : 0);
}