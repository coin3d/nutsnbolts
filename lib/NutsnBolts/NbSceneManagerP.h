#ifndef NB_SCENEMANAGERP_H
#define NB_SCENEMANAGERP_H

/**************************************************************************\
 *
 *  This file is part of the SIM Nuts'n'Bolts extension library for Coin.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License ("GPL") version 2
 *  as published by the Free Software Foundation.  See the file COPYING
 *  at the root directory of this source distribution for additional
 *  information about the GNU GPL.
 *
 *  For using SIM Nuts'n'Bolts with software that can not be combined with
 *  the GNU GPL, and for taking advantage of the additional benefits of
 *  our support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion AS, Bygdøy allé 5, N-0257 Oslo, NORWAY. (www.sim.no)
 *
\**************************************************************************/


#include <Inventor/SbBasic.h>
#include <Inventor/SbColor.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/elements/SoLazyElement.h>

class SoInfo;
class SoCamera;
class NbSceneManager;
class NbNavigationSystem;
class SoGLRenderAction;
class SoGetMatrixAction;
class SoGetBoundingBoxAction;

class NbSceneManagerP {
public:
  NbSceneManagerP(NbSceneManager * api);
  ~NbSceneManagerP(void);

  void clearBuffers(SbBool color, SbBool depth);
  SoCamera * getCamera(void);
  void touch(void);
  void renderSingle(SoGLRenderAction * action,
                    SbBool initmatrices,
                    SbBool clearwindow,
                    SbBool clearzbuffer);

  NbNavigationSystem * searchForNavigationMode(SoNode * root, 
                                               NbNavigationSystem * defsys);
  
  

public:
  NbSceneManager::RenderMode rendermode;
  NbSceneManager::StereoMode stereomode;

  SbBool texturesenabled;
  float stereooffset;
  SoInfo * dummynode;
  SoSearchAction searchaction;
  SoCamera * camera;
  SoColorPacker colorpacker;
  SbColor overlaycolor;

  SbPList superimpositions;

  NbSceneManager::NavigationState navigationstate;
  NbNavigationSystem * navigationsystem;

  SoGetBoundingBoxAction * getbboxaction;
  SoGetMatrixAction * getmatrixaction;

  int depthbits;
  NbSceneManager::AutoClippingStrategy autoclipping;
  float nearplanevalue;
  SoNodeSensor * autoclipsensor;
  static void update_clipping_planes(void * cosure, SoSensor * sensor);

  void setClippingPlanes(void);
  void getCameraCoordinateSystem(SbMatrix & matrix, SbMatrix & inverse);

protected:
  NbSceneManager * master;

}; 

class Superimposition {
public:
  Superimposition(SoNode * scene,
                  SbBool enabled,
                  SbBool autoredraw,
                  SbBool zbufferon,
                  SoSceneManager * manager);
  ~Superimposition();
  
  void render(void);
  
private:
  static void changeCB(void * data, SoSensor * sensor);
  
  SoNode * scene;
  SbBool enabled;
  SbBool autoredraw;
  SbBool zbufferon;
  SbBool zbufferwason;
  SoSceneManager * manager;
  SoNodeSensor * sensor;
};

#endif // NB_SCENEMANAGERP_H

