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

#include <assert.h>
#include <stdio.h>

#include <Inventor/events/SoLocation2Event.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>
#include <NutsnBolts/navigation/NbYawMode.h>

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbYawMode);

void
NbYawMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbYawMode, NbNavigationMode);
}

NbYawMode::NbYawMode(SbName name)
 : inherited(name)
{
  PRIVATE(this) = NULL;
  this->set1DValueFunc(NbNavigationMode::getMouseMoveVerticalNormalizedDistance, NULL);
}

NbYawMode::~NbYawMode(void)
{
}

NbNavigationMode *
NbYawMode::clone(void) const
{
  return new NbYawMode(this->getModeName());
}

SbBool
NbYawMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }
  ctrl->restoreCamera();
  float value = this->get1DValue(ctrl);
  ctrl->yawCamera(value);
  return TRUE;
}

#undef PRIVATE
