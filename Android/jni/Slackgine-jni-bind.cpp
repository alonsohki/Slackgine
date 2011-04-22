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
#include <string.h>
#include <jni.h>
#include "../../Test/l3mWithDescription.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/HelloJni/HelloJni.java
 */
JNIEXPORT jstring JNICALL
Java_es_lautech_slackgine_Slackgine_PruebaString( JNIEnv* env,
                                                  jobject thiz )
{
    char ret [ 512 ];
    float fVertices [] =
    {
        // Position  
        0.0f, 1.0f, 0.5f,
        -0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
    };
    unsigned int faces [] =
    {
        0, 1, 2
    };
    
    l3mWithDescription model ( "Triángulo cromático demostrativo de la interpolación de colores de OpenGL" );;
    Mesh* mesh = Mesh::LoadAllocating("triangle", fVertices, Vertex::LOAD_POSITION, 0, 3, faces, 3 );
    model.LoadMesh(mesh, "main");

    if ( model.SaveToFile ( "/sdcard/chromatic_tri.l3m" ) != l3m::OK )
        sprintf ( ret, "Error al guardar el fichero: %s\n", model.error() );
    else
    {
    l3mWithDescription model2;
    if ( model2.LoadFromFile ( "/sdcard/chromatic_tri.l3m" ) != l3m::OK )
        sprintf ( ret, "Error al cargar el fichero: %s\n", model2.error() );
    else
      sprintf ( ret, "Descripción leída: %s\n", model2.description().c_str() );
    }

  return env->NewStringUTF(ret);
}

#ifdef __cplusplus
}
#endif

