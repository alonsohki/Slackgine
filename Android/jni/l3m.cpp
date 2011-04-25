/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <jni.h>
#include "slackgine.h"

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT jlong    JNICALL   Java_es_lautech_slackgine_l3m_Createl3mInstance(JNIEnv*, jobject);
    JNIEXPORT jlong    JNICALL   Java_es_lautech_slackgine_l3m_Createl3mInstance_type(JNIEnv*, jobject, jstring);
    JNIEXPORT void     JNICALL   Java_es_lautech_slackgine_l3m_Destroyl3mInstance(JNIEnv*, jobject, jlong);
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_InternalLoad(JNIEnv*, jobject, jbyteArray, jint); 
    JNIEXPORT jboolean JNICALL   Java_es_lautech_slackgine_l3m_InternalSave(JNIEnv*, jobject, jobject);
}
#endif

class javal3m : public l3m
{
public:
    javal3m () : l3m () {}
    javal3m ( const char* type ) : l3m ( type ) {}
    ~javal3m () {}
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
    javal3m* instance = new javal3m ();
    return reinterpret_cast<long> ( instance );
}

JNIEXPORT jlong JNICALL
Java_es_lautech_slackgine_l3m_Createl3mInstance_type ( JNIEnv* env, jobject thiz, jstring type )
{
    const char* typestr = env->GetStringUTFChars ( type, 0 );
    javal3m* instance = new javal3m ( typestr );
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
Java_es_lautech_slackgine_l3m_InternalLoad ( JNIEnv* env, jobject thiz, jbyteArray bytes, jint size )
{
    javal3m* instance = Getl3m ( env, thiz );
    jbyte* data = env->GetByteArrayElements ( bytes, 0 );
    std::istringstream stream ( (char *)data, size );
    if ( instance->Load ( stream ) == l3m::OK )
    {
        env->ReleaseByteArrayElements ( bytes, data, 0 );
        return true;
    }
    return false;
}

JNIEXPORT jboolean JNICALL
Java_es_lautech_slackgine_l3m_InternalSave ( JNIEnv* env, jobject thiz, jobject internal_os )
{
    javal3m* instance = Getl3m ( env, thiz );
    jclass ByteArrayOutputStreamClass = env->FindClass ( "java.io.ByteArrayOutputStream" );
    jmethodID writeID = env->GetMethodID ( ByteArrayOutputStreamClass, "write", "([BII)V" );
    
    std::ostringstream os;
    if ( instance->Save ( os ) == l3m::OK )
    {
        jbyteArray array = env->NewByteArray ( os.str().size () );
        env->SetByteArrayRegion ( array, 0, os.str().size(), (const jbyte *)os.str().c_str() );
        env->CallVoidMethod ( internal_os, writeID, array, 0, os.str().size() );
        return true;
    }

    return false;
}

