#ifndef NB_NAVIGATIONSTATE_H
#define NB_NAVIGATIONSTATE_H

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

#include <NutsnBolts/Basic.h>

class SoEvent;
class NbNavigationMode;

class NbNavigationStateP;

class NB_DLL_API NbNavigationState {
public:
  NbNavigationState(void);
  ~NbNavigationState(void);

  void push(NbNavigationMode * mode, const SoEvent * trigger);
  void pop(void);
  void reset(void);

  NbNavigationMode * getMode(void) const;
  const SoEvent * getTrigger(void) const;

private:
  NbNavigationStateP * pimpl;

}; // NbNavigationState

#endif // !NB_NAVIGATIONSTATE_H
