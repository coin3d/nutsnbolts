#ifndef NB_PICKMODE_H
#define NB_PICKMODE_H

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

#include <NutsnBolts/navigation/NbNavigationMode.h>

class NbPickModeP;

typedef void NbPickModePickCB(void * closure, const SbVec3f & pos, const SoPath * path);

class NB_DLL_API NbPickMode : public NbNavigationMode {
  typedef NbNavigationMode inherited;
  NB_NAVIGATION_MODE_HEADER(NbPickMode);

public:
  static void initClass(void);

  NbPickMode(SbName name);
  virtual ~NbPickMode(void);

  virtual NbNavigationMode * clone(void) const;

  void addPickCallback(NbPickModePickCB * cb, void * closure);
  void removePickCallback(NbPickModePickCB * cb, void * closure);

protected:
  virtual SbBool handleEvent(const SoEvent * event, 
                             const NbNavigationControl * ctrl);

  void invokePickCallbacks(const SbVec3f & pos, const SoPath * path);

private:
  NbPickModeP * pimpl;

}; // NbPitchMode

#endif // !NB_PICKMODE_H
