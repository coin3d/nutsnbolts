#ifndef NB_SCENEMANAGER_H
#define NB_SCENEMANAGER_H

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
