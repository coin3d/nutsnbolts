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

#include <Inventor/SbVec3f.h>
#include <Inventor/SbPlane.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCamera.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>

#include <NutsnBolts/navigation/NbOrthoCenterMode.h>

// *************************************************************************

/*!
  \class NbCenterMode NutsnBolts/navigation/NbCenterMode.h
  \brief Class that implements a focalpoint centering mode.

  This class implements the focalpoint centering mode, moving the camera
  without changing the view axis.

  \ingroup navigation
*/

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbOrthoCenterMode);

void
NbOrthoCenterMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbOrthoCenterMode, NbNavigationMode);
}

/*!
  Constructor.
*/

NbOrthoCenterMode::NbOrthoCenterMode(SbName name)
  : inherited(name)
{
  // we don't have the need for a private implementation, but we have
  // set off space for one if we ever need one in the future.
  PRIVATE(this) = NULL;
}

/*!
  Destructor.
*/

NbOrthoCenterMode::~NbOrthoCenterMode(void)
{
}

/*!
*/

NbNavigationMode *
NbOrthoCenterMode::clone(void) const
{
  return new NbOrthoCenterMode(this->getModeName());
}

/*!
  This method implements the camera move operation.

  Returns FALSE for events that are not used, and TRUE for events that
  are used.
*/

SbBool
NbOrthoCenterMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (! event->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    return FALSE;
  }
  SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) event;
  if (mbevent->getState() != SoButtonEvent::DOWN) {
    return FALSE;
  }

  SoCamera * camera = ctrl->getCamera();
  if (!camera) {
    return FALSE;
  }

  SbVec3f pickpos;
  SoPath * path = ctrl->pick(event->getPosition(), pickpos);

  if (path != NULL) {
    SbRotation rot = camera->orientation.getValue();
    SbVec3f up(0, 1, 0);
    SbVec3f right(1, 0, 0);
    SbVec3f pos = camera->position.getValue();
    rot.multVec(up, up);
    rot.multVec(right, right);

    SbPlane plane(pos, pos+up, pos+right);

    float distance = plane.getDistance(pickpos);
    SbVec3f normal = plane.getNormal();
    normal.normalize();

    SbVec3f newpos = pickpos - (normal * distance);
    camera->position.setValue(newpos);
    // fprintf(stderr, "orthomode move from <%f %f %f> to <%f %f %f>\n",
    //         pos[0], pos[1], pos[2], newpos[0], newpos[1], newpos[2]);
    this->finish();
  } else {
    this->abort();
  }
  return TRUE;
}

#undef PRIVATE

// *************************************************************************
