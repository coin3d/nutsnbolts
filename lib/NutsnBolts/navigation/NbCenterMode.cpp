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

#include <stdio.h>
#include <assert.h>

#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>

#include <NutsnBolts/navigation/NbCenterMode.h>

// *************************************************************************

/*!
  \class NbCenterMode NutsnBolts/navigation/NbCenterMode.h
  \brief Class that implements the focalpoint centering mode.

  This class implements the focalpoint centering mode.

  \ingroup navigation
*/

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbCenterMode);

void
NbCenterMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbCenterMode, NbNavigationMode);
}

/*!
  Constructor.
*/

NbCenterMode::NbCenterMode(SbName name)
  : inherited(name)
{
  // we don't have the need for a private implementation, but we have
  // set off space for one if we ever need one in the future.
  PRIVATE(this) = NULL;
}

/*!
  Destructor.
*/

NbCenterMode::~NbCenterMode(void)
{
}

/*!
*/

NbNavigationMode *
NbCenterMode::clone(void) const
{
  return new NbCenterMode(this->getModeName());
}

/*!
  This method implements the panning operation.

  Returns FALSE for events that are not used, and TRUE for events that
  are used.
*/

SbBool
NbCenterMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (! event->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    return FALSE;
  }
  SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) event;
  if (mbevent->getState() != SoButtonEvent::DOWN) {
    return FALSE;
  }

  SbVec3d pickpos;
  SoPath * path = ctrl->pick(event->getPosition(), pickpos);
  if (path != NULL) {
    SbVec3f pos((float) pickpos[0], (float) pickpos[1], (float) pickpos[2]);
    ctrl->reorientCamera(pos);
    // FIXME: this part should be factored out of this mode
    ctrl->moveCamera(0.20f, TRUE); // zoom in 20%
    this->finish();
  } else {
    this->abort();
  }
  return TRUE;
}

#undef PRIVATE

// *************************************************************************
