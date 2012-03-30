/////////////////////////////////////////////////////////////
//
// Slackgine - Copyright (C) 2010-2011
// The Slackgine development team
//
// See the LICENSE file in the top-level directory.
//
// FILE:        main.cpp
// PURPOSE:     Main test file.
// AUTHORS:     Alberto Alonso <rydencillo@gmail.com>
//

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "slackgine.h"
#include "l3m/l3m.h"
#include "renderer/strategies/default.h"

const char * DEFAULT_MODEL_PATH = "models";
const char * DEFAULT_SHADER_PATH = "shaders";
const char * DEFAULT_MODEL_FILENAME = "head4.l3m";

void display ( void );

using namespace Core;

static Entity* entity = 0;
static l3m::Model* model = 0;
static Slackgine* sg = 0;
static Camera* cam = 0;
static l3m::Morph* morph = 0;
static bool singleShapeMode = true;
static bool autoShapeCircling = true;

static void morphingTestKeyPress(unsigned char key, int x, int y)
{
  const u32 MAX_ACTIVE_SHAPES = Renderer::Morph::MAX_ACTIVE_SHAPES;
  bool isDecrem = (glutGetModifiers() & GLUT_ACTIVE_ALT);
  bool isActive = false;
  float* weights = morph->morph().shapeWeights();
  u32* activeShape = morph->morph().activeShapes();
  u32 numActiveShapes = morph->morph().numActiveShapes();
  int shape = -1;
  float weight;
  
  const float dWeight = 0.05f;
  const float zeroTol = 0.001f;
  const float oneTol = 0.999f;
  
  switch (key)
  {
    case 'a':
    case 'A':
      autoShapeCircling = !autoShapeCircling;
      printf("Automatic Sape Circling: %s!\n", 
        autoShapeCircling? "enabled" : "disabled");
      break;
      
    case 's':
    case 'S':
      singleShapeMode = !singleShapeMode;
      printf("Single Shape Mode: %s\n", 
        singleShapeMode? "enabled" : "disabled");
      break;
      
    default:
      if ( key >= '1' && key <= '9' ) {
        shape = (key - '1');
        if (shape < morph->morph().numShapes()) {
          weight = weights[shape];
          weight += (isDecrem)? -dWeight : dWeight;
          weight = (weight > zeroTol)? weight : 0.0f;
          weight = (weight < oneTol )? weight : 1.0f;
        }
        else {
          shape = -1;
        }
      }
  }
  
  if (shape >= 0) {
    autoShapeCircling = false;
      
    // single shape mode: only one shape can be active 
    if (singleShapeMode)
    {
      if (numActiveShapes > 0) {
        if (activeShape[0] == shape) {
          weights[shape] = weight;
        }
        else {
          weights[activeShape[0]] -= dWeight;
          if (weights[activeShape[0]] < zeroTol) {
            weights[activeShape[0]] = 0.0f;
            numActiveShapes--;
          }
        }
      }
      
      if (numActiveShapes > 1) {
        numActiveShapes = 1;
      }
      
      if (numActiveShapes == 0 && !isDecrem) {
        activeShape[numActiveShapes++] = shape;
        weights[shape] = dWeight;        
      }

      morph->morph().numActiveShapes() = numActiveShapes;
      
      printf("Single Shape Mode: %.2f [Shape %d] '%s'\n",
        morph->morph().shapeWeights()[activeShape[0]], activeShape[0]+1,
        morph->morph().shapeNames()[activeShape[0]].c_str());
      fflush(stdout);
    }
    else // single shape mode disabled
    {
      isActive = false;
      for ( u32 i = 0; i < numActiveShapes; i++ )
      {
        if (shape == activeShape[i]) {
          weights[shape] = weight;
          isActive = true;
        }
        if (weights[activeShape[i]] < zeroTol) {
          weights[activeShape[i]] = 0.0f;
          for ( u32 j = i+1; j < numActiveShapes; j++ ) {
            activeShape[i] = activeShape[j];
          }
          numActiveShapes--;
        }
      }

      if (!isActive && !isDecrem && numActiveShapes < MAX_ACTIVE_SHAPES) {
        activeShape[numActiveShapes++] = shape;
        weights[shape] = dWeight;
      
      }
      morph->morph().numActiveShapes() = numActiveShapes;
      
      if (numActiveShapes > 0) {
        printf("Active Shapes:\n");
        for ( u32 i = 0; i < numActiveShapes; i++ ) {
          printf("  %d: %.2f [Shape %2d] '%s'\n",
            i+1, morph->morph().shapeWeights()[activeShape[i]],
            activeShape[i]+1,
            morph->morph().shapeNames()[activeShape[i]].c_str());
        }
      }      
      
    }
  }
}

static void doAutoShapeCircling()
{
  static bool isIncrementing = true;
  const float weightThreshold = 0.97f;
  const float dWeightSlow = 0.001f;
  const float dWeight = 0.02f;
  float* weights = morph->morph().shapeWeights();
  u32 shape = morph->morph().activeShapes()[0];
  bool shapeCircled = false;
  
  if (morph->morph().numActiveShapes() < 1) {
    shape = 0;
    morph->morph().activeShapes()[0] = shape;
  }
  morph->morph().numActiveShapes() = 1;
  
  float weight = weights[shape];    
  
  if (isIncrementing) {
    if (weight > weightThreshold)
      weight += dWeightSlow;
    else
      weight += dWeight;
    if (weight > 1.0f) {
      weight = 1.0f;
      isIncrementing = false;
    }
  }
  else {
    weight -= dWeight;
    if (weight < 0.0f) {
      weight = 0.0f;
      isIncrementing = true;
      shapeCircled = true;
    }
  }
  weights[shape] = weight;

  if (shapeCircled) {
    shape++;
    if (shape >= morph->morph().numShapes())
      shape = 0;
    weights[shape] = 0.0f;
    morph->morph().activeShapes()[0] = shape;
  }
  
  sg->getTime().sleepMS(10);
}


static void doMorphingTest()
{  
  if (morph == 0 && model)
  {
    printf("Starting morphing test\n");
    for ( l3m::Model::ComponentVector::iterator iter = model->components().begin();
          iter != model->components().end();
          ++iter )
    {
      if ( (*iter)->type() == "morph" ) {
        morph = static_cast<l3m::Morph*>(*iter);
        break;
      }
    }
    
    if (morph) {
      printf("Morph object found:\n");
      for (int i=0; i<morph->morph().numShapes(); i++) {
        printf("  Shape %2d: %.3f '%s'\n",
          i+1, morph->morph().shapeWeights()[i],
          morph->morph().shapeNames()[i].c_str());
      }
      
      glutKeyboardFunc(morphingTestKeyPress);
    }
    else {
      printf("Morph object not found!\n");
    }
  }
  
  if (morph && autoShapeCircling) {
    doAutoShapeCircling();
  }
}
    
static void cleanup ()
{
    if ( sg != 0 )
    {
        sgDelete sg;
        sg = 0;
    }
}

static void sighandler ( int signum )
{
    switch ( signum )
    {
        case SIGINT:
        {
            cleanup ();
            exit(0);
            break;
        }
    }
}

int main(int argc, char** argv)
{
    signal ( SIGINT, sighandler );

    glutInit (&argc, argv);
    glutInitWindowSize (1920/2, 1080/2);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
    glutCreateWindow ("Slackgine test");
    
    glutDisplayFunc ( display );
    atexit ( cleanup );
    
    glutMainLoop ();
    
    sgDelete entity;
    
    return EXIT_SUCCESS;
}


void display ( void )
{
    if ( sg == 0 )
    {
        sg = sgNew Slackgine ();
        sg->initialize ();
        sg->getModelManager().addLookupPath ( DEFAULT_MODEL_PATH );
        sg->getShaderManager().addLookupPath( DEFAULT_SHADER_PATH );
        sg->setRenderStrategy ( sgNew Renderer::Strategy::Default () );
    }

    if ( model == 0 )
    {
        model = sg->getModelManager().requestBlocking ( DEFAULT_MODEL_FILENAME );
        if ( model->isOk() == true )
        {
            l3m::Scene* sce = l3m::Util::findScene(model);
            if ( sce != 0 )
                glutReshapeWindow( sce->width(), sce->height() );
            if ( entity != 0 )
                entity->setModel ( model );
        }
        else
        {
            fprintf ( stderr, "Error loading model: %s\n", model->error() );
        }
    }
    if ( entity == 0 )
    {
        entity = sgNew Entity ( model, &sg->getWorld() );
        l3m::Scene* sce = l3m::Util::findScene ( model );
        if ( sce != 0 )
        {
            cam = sgNew Camera ( sce->camera() );
        }
    }
    
    sg->tick ();
    glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    char error [ 1024 ];
    if ( !sg->beginScene(cam) )
    {
        sg->getError(error);
        fprintf ( stderr, "Error beginning the scene: %s\n", error );
    }
    else if ( !sg->render() )
    {
        sg->getError ( error );
        fprintf ( stderr, "Error rendering the scene: %s\n", error );
    }
    else if ( !sg->endScene() )
    {
        sg->getError(error);
        fprintf ( stderr, "Error ending the scene: %s\n", error );
    }
    
    // do a morphing test
    doMorphingTest();
    
    glutSwapBuffers ();
    glutPostRedisplay ();
}
