#ifndef NB_SCENEMANAGER_H
#define NB_SCENEMANAGER_H

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

#include <Inventor/SoSceneManager.h>
#include <NutsnBolts/NutsnBolts.h>

class SoCamera;
class NbNavigationSystem;
class NbNavigationMode;
class NbSceneManagerP;

class NB_DLL_API NbSceneManager : public SoSceneManager {
  typedef SoSceneManager inherited;

public:
  NbSceneManager(void);
  virtual ~NbSceneManager(void);

  virtual void render(const SbBool clearwindow = TRUE,
                      const SbBool clearzbuffer = TRUE);
  virtual void render(SoGLRenderAction * action,
                      const SbBool initmatrices = TRUE,
                      const SbBool clearwindow = TRUE,
                      const SbBool clearzbuffer = TRUE);
  
  virtual void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;

  enum RenderMode {
    AS_IS,
    WIREFRAME,
    POINTS,
    WIREFRAME_OVERLAY,
    HIDDEN_LINE,
    BOUNDING_BOX
  };
  
  virtual void setRenderMode(RenderMode mode);
  RenderMode getRenderMode(void) const;

  enum StereoMode {
    MONO,
    RED_CYAN,
    RED_BLUE,
    QUAD_BUFFER,
    INTERLEAVED_ROWS,
    INTERLEAVED_COLUMNS
  };

  virtual void setStereoMode(StereoMode mode);
  StereoMode getStereoMode(void) const;

  virtual void setStereoOffset(float offset);
  float getStereoOffset(void) const;
  
  virtual void setTexturesEnabled(const SbBool onoff);
  SbBool isTexturesEnabled(void) const;

  virtual void setWireframeOverlayColor(const SbColor & color);
  const SbColor & getWireframeOverlayColor(void) const;

  virtual SbBool processEvent(const SoEvent * const event);

  enum NavigationState {
    NO_NAVIGATION,
    JUST_NAVIGATION,
    MIXED_NAVIGATION
  };

  virtual void setNavigationState(NavigationState state);
  virtual NavigationState getNavigationState(void) const;

  virtual void setNavigationSystem(NbNavigationSystem * system);
  virtual NbNavigationSystem * getNavigationSystem(void) const;

  virtual void setSceneGraph(SoNode * const root);
  virtual SoNode * getSceneGraph(void) const;

  virtual void activate(void);
  virtual void deactivate(void);

  enum AutoClippingStrategy {
    NO_AUTO_CLIPPING,
    FIXED_NEAR_PLANE,
    VARIABLE_NEAR_PLANE
  };

  virtual void setAutoClipping(AutoClippingStrategy strategy);
  virtual AutoClippingStrategy getAutoClipping(void) const;

  virtual void setNearPlaneValue(float value);
  virtual float getNearPlaneValue(void) const;

private:
  NbSceneManagerP * pimpl;

}; // NbSceneManager

#endif // !NB_SCENEMANAGER_H
