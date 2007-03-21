#ifndef NB_NAVIGATIONMODE_H
#define NB_NAVIGATIONMODE_H

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

#include <Inventor/SbName.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbViewportRegion.h>

#include <NutsnBolts/Basic.h>
#include <NutsnBolts/navigation/NbSubMode.h>

class SbRotation;
class SoNode;
class SoCamera;
class SoEvent;

class NbNavigationSystem;
class NbNavigationControl;

class NbNavigationMode;
class NbNavigationModeP;

typedef float NbNavigation1DInputValueFunc(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
typedef SbVec2f NbNavigation2DInputValueFunc(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
typedef SbVec3f NbNavigation3DInputValueFunc(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);

class NB_DLL_API NbNavigationMode {
  NB_NAVIGATION_MODE_HEADER(NbNavigationMode);

public:
  static void initClass(void);

  // predefined set of input value functions
  static float getAbsMouseMovePixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveVerticalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveVerticalNormalizedDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveHorizontalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveHorizontalNormalizedDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getAbsMouseCenterPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseCenterVerticalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseCenterHorizontalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseCenterAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveCenterAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);

public:
  NbNavigationMode(SbName modename);
  virtual ~NbNavigationMode(void);

  virtual NbNavigationMode * clone(void) const = 0;

  SbName getModeName(void) const;

  void set1DValueFunc(NbNavigation1DInputValueFunc * func, void * closure);
  NbNavigation1DInputValueFunc * get1DValueFunc(void ** closureptr = NULL) const;
  void set2DValueFunc(NbNavigation2DInputValueFunc * func, void * closure);
  NbNavigation2DInputValueFunc * get2DValueFunc(void ** closureptr = NULL) const;
  void set3DValueFunc(NbNavigation3DInputValueFunc * func, void * closure);
  NbNavigation3DInputValueFunc * get3DValueFunc(void ** closureptr = NULL) const;

  SbBool processEvent(const SoEvent * event, const NbNavigationControl * ctrl);

  virtual SoNode * getSceneGraph(void);

  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationControl * ctrl) = 0;

  virtual void init(const SoEvent * event, const NbNavigationControl * ctrl);
  virtual void abort(const SoEvent * event, const NbNavigationControl * ctrl);
  virtual void finish(const SoEvent * event, const NbNavigationControl * ctrl);

  SbBool isAborted(void) const;
  SbBool isFinished(void) const;

protected:
  float get1DValue(const NbNavigationControl * ctrl) const;
  SbVec2f get2DValue(const NbNavigationControl * ctrl) const;
  SbVec3f get3DValue(const NbNavigationControl * ctrl) const;

  SbVec2s getInitialPosition(void) const;
  SbVec2s getPreviousPosition(void) const;
  SbVec2s getCurrentPosition(void) const;

  SbVec2f getInitialNormalizedPosition(const NbNavigationControl * ctrl) const;
  SbVec2f getPreviousNormalizedPosition(const NbNavigationControl * ctrl) const;
  SbVec2f getCurrentNormalizedPosition(const NbNavigationControl * ctrl) const;

  void abort(void);
  void finish(void);

private:
  NbNavigationModeP * pimpl;

}; // NbNavigationMode

#endif // !NB_NAVIGATIONMODE_H
