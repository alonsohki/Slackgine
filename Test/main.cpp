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

const char * DEFAULT_MODEL_PATH = "..";
const char * DEFAULT_SHADER_PATH = "../shaders";
const char * DEFAULT_MODEL_FILENAME = "spherecube.l3m";

void display ( void );

using namespace Core;

static Entity* entity = 0;
static l3m::Model* model = 0;
static Slackgine* sg = 0;
static Camera* cam = 0;
static l3m::Morph* morph = 0;
static bool singleShapeMode = true;
static bool autoShapeCircling = false;

static void morphingTestKeyPress(unsigned char key, int x, int y)
{
  const u32 MAX_ACTIVE_SHAPES = Renderer::Morph::MAX_ACTIVE_SHAPES;
  bool isDecrem = (glutGetModifiers() & GLUT_ACTIVE_ALT);

  Renderer::Morph & mo = morph->morph();
  u32* activeShape = mo.activeShapes();
  f32* activeWeights = mo.activeWeights();
  int shape = -1;
  
  const float dWeight = 0.05f;
  const float zeroTol = 0.001f;
  const float oneTol = 0.999f;
  
  switch (key)
  {
    case 'a':
    case 'A':
      autoShapeCircling = !autoShapeCircling;
      printf("Automatic Shape Circling: %s!\n", 
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
        if (shape >= mo.numShapes()) {
          shape = -1;
        }
      }
  }
  
  if (shape >= 0) {
    autoShapeCircling = false;
      
    float weight = (isDecrem)? 0.0f:dWeight;
    int index = mo.findActiveShapeIndex(shape);
    if (index >= 0)
    {
      weight = activeWeights[index];
      weight += (isDecrem)? -dWeight:dWeight;
      if (weight > oneTol)
        weight = 1.0f;
      else if (weight < zeroTol)
        weight = 0.0f;
    }
        
    // single shape mode: only one shape can be active 
    if (singleShapeMode)
    {
      if (mo.numActiveShapes() == 0)
      {
        if (!isDecrem)
          mo.addActiveShape(shape, weight);
      }
      else if (mo.numActiveShapes() > 0)
      {
        mo.numActiveShapes() = 1;
        
        if (activeShape[0] == shape) {
          mo.setActiveShapeWeight(0, weight);
        }
        else if (!isDecrem) {
          activeWeights[0] -= dWeight;
          if (activeWeights[0] < zeroTol) {
            activeWeights[0] = 0;
            mo.reset();
          }
        }
      }
      
      if (mo.numActiveShapes() > 0) {
        printf("Single Shape Mode: %.2f [Shape %d] '%s'\n",
          activeWeights[0], activeShape[0]+1,
          mo.shapeNames()[activeShape[0]].c_str());
      }
    }
    else // single shape mode disabled
    {
      if (!mo.setShapeWeight(shape, weight)) {
        printf("Cannot activate more shapes, limit reached!\n");
      }
      
      for ( u32 i = 0; i < mo.numActiveShapes(); i++ ) {
        if (activeWeights[i] < zeroTol) {
          mo.disableActiveShape(i);
        }
      }

      if (mo.numActiveShapes() > 0) {
        printf("Active Shapes:\n");
        for ( u32 i = 0; i < mo.numActiveShapes(); i++ ) {
          printf("  %d: %.2f [Shape %2d] '%s'\n",
            i+1, activeWeights[i], activeShape[i]+1,
            mo.shapeNames()[activeShape[i]].c_str());
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
  
  Renderer::Morph& mo = morph->morph();
  bool shapeCircled = false;
  
  mo.numActiveShapes() = 1;
  if (mo.activeShapes()[0] > mo.numShapes()) {
    mo.activeShapes()[0] = 0;
  }
    
  float weight = mo.activeWeights()[0];
  
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
  mo.activeWeights()[0] = weight;

  if (shapeCircled) {
    u32 shape = mo.activeShapes()[0];
    if (++shape >= mo.numShapes())
      shape = 0;

    mo.activeShapes()[0] = shape;
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
      Renderer::Morph& mo = morph->morph();
       
      printf("Morph object found:\n");
      for (int i=0; i<mo.numShapes(); i++) {
        printf("  Shape %2d: '%s'\n",
          i+1, mo.shapeNames()[i].c_str());
      }
      
      for (int i=0; i<mo.numActiveShapes(); i++) {
        int shape = mo.activeShapes()[i];
        printf("  Active Shape %2d -> %2d '%s', weight = %.3f\n",
          i+1, shape+1, mo.shapeNames()[shape].c_str(), mo.activeWeights()[i]);
      }
      
      printf("Control keys:\n"
             "       a -> Toggle automatic shape circling\n"
             "       s -> Toggle single shape mode\n"
             "     1-9 -> Increase weight of shape N\n"
             " ALT 1-9 -> Decrease weight of shape N\n");
          
      glutKeyboardFunc(morphingTestKeyPress);           
    }
    else {
      printf("Morph object not found!\n");
    }
  }

  if (autoShapeCircling && morph) {
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
