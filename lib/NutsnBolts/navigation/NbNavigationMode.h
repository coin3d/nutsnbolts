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
#include <Inventor/SbViewportRegion.h>

#include <NutsnBolts/Basic.h>

class SbRotation;
class SoNode;
class SoCamera;
class SoEvent;

class NbNavigationSystem;
class NbNavigationInfo;

class NbNavigationModeP;

class NB_DLL_API NbNavigationMode {
public:
  static void initClass(void);
  static void cleanClass(void);

public:
  NbNavigationMode(SbName modename);
  virtual ~NbNavigationMode(void);

  SbName getModeName(void) const;

  SbBool processEvent(const SoEvent * event, const NbNavigationInfo * info);

  virtual SoNode * getSceneGraph(void);

  void addTransition(NbNavigationMode * newmode,
		     const SoEvent * event,
		     SbBool stack = TRUE,
		     const SoEvent * condition = NULL);
  void addAbort(const SoEvent * event, const SoEvent * condition = NULL);
  void addFinish(const SoEvent * event, const SoEvent * condition = NULL);

  void activate(void);
  void deactivate(void);
  SbBool isActive(void) const;

  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationInfo * info) = 0;

  virtual void init(const SoEvent * event, const NbNavigationInfo * info);
  virtual void abort(const SoEvent * event, const NbNavigationInfo * info);
  virtual void finish(const SoEvent * event, const NbNavigationInfo * info);

protected:
  SbVec2s getInitialPosition(void) const;
  SbVec2s getPreviousPosition(void) const;
  SbVec2s getCurrentPosition(void) const;

  SbVec2f getInitialNormalizedPosition(const NbNavigationInfo * info) const;
  SbVec2f getPreviousNormalizedPosition(const NbNavigationInfo * info) const;
  SbVec2f getCurrentNormalizedPosition(const NbNavigationInfo * info) const;

private:
  NbNavigationModeP * pimpl;

}; // NbNavigationMode

#endif // !NB_NAVIGATIONMODE_H
