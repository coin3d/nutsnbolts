#ifndef NB_SCENEMANAGERP_H
#define NB_SCENEMANAGERP_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#endif // NB_SCENEMANAGERP_H

