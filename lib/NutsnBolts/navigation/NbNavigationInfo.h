#ifndef NB_NAVIGATIONINFO_H
#define NB_NAVIGATIONINFO_H

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

#include <Inventor/SbVec2s.h>

#include <NutsnBolts/Basic.h>

class SoCamera;
class SoNode;
class SbRotation;
class SbViewportRegion;

class NbNavigationInfoP;

class NB_DLL_API NbNavigationInfo {
public:
  NbNavigationInfo(void);
  virtual ~NbNavigationInfo(void);

  void setSceneGraph(SoNode * scene);
  SoNode * getSceneGraph(void) const;

  void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;

  void saveCamera(void) const;
  void restoreCamera(void) const;

  void reorientCamera(const SbRotation & rot) const;
  void moveCamera(const SbVec3f & vec) const;

  void setViewport(const SbViewportRegion & vp);
  SbVec2s getViewportSize(void) const;
  float getViewportAspect(void) const;

protected:

private:
  NbNavigationInfoP * pimpl;

}; // NbNavigationInfo

#endif  // !NB_NAVIGATIONINFO_H
