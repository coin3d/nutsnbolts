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
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SoPickedPoint.h>

#include <NutsnBolts/navigation/NbNavigationInfo.h>

#include <NutsnBolts/navigation/NbCenterMode.h>

// *************************************************************************

/*!
  \class NbCenterMode NutsnBolts/navigation/NbCenterMode.h
  \brief Class that implements the focalpoint centering mode.

  This class implements the focalpoint centering mode.

  \ingroup navigation
*/

// *************************************************************************

class NbCenterModeP {
public:
  NbCenterModeP(NbCenterMode * api);

  SoRayPickAction * rpaction;

};

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/

NbCenterMode::NbCenterMode(SbName name)
  : inherited(name)
{
  // we don't have the need for a private implementation, but we have
  // set off space for one if we ever need one in the future.
  PRIVATE(this) = new NbCenterModeP(this);
}

/*!
  Destructor.
*/

NbCenterMode::~NbCenterMode(void)
{
  if ( PRIVATE(this)->rpaction ) {
    delete PRIVATE(this)->rpaction;
    PRIVATE(this)->rpaction = NULL;
  }
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  This method implements the panning operation.

  Returns FALSE for events that are not used, and TRUE for events that
  are used.
*/

SbBool
NbCenterMode::handleEvent(const SoEvent * event, const NbNavigationInfo * info)
{
  if ( ! event->isOfType(SoMouseButtonEvent::getClassTypeId()) ) {
    return FALSE;
  }
  SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) event;
  if ( mbevent->getState() != SoButtonEvent::DOWN ) {
    return FALSE;
  }

  SoCamera * camera = info->getCamera();
  if ( !camera ) {
    return FALSE;
  }

  SoNode * scene = info->getSceneGraph();
  if ( !scene ) {
    return FALSE;
  }

  SbViewportRegion vp;
  vp.setWindowSize(info->getViewportSize());

  if ( !PRIVATE(this)->rpaction ) {
    PRIVATE(this)->rpaction = new SoRayPickAction(vp);
  } else {
    PRIVATE(this)->rpaction->reset();
    PRIVATE(this)->rpaction->setViewportRegion(vp);
  }
  PRIVATE(this)->rpaction->setPoint(event->getPosition());

  PRIVATE(this)->rpaction->apply(scene);

  SoPickedPoint * pp = PRIVATE(this)->rpaction->getPickedPoint();
  if ( !pp ) {
    PRIVATE(this)->rpaction->reset();
    return FALSE;
  }

  // FIXME: collect up all the relevant matrices, if necessary
  SbVec3f point = pp->getPoint();

  SbRotation rot = camera->orientation.getValue();
  SbVec3f up;
  rot.multVec(SbVec3f(0, 1, 0), up);
  SbVec3f vec = point - camera->position.getValue();

  camera->pointAt(point, up);
  camera->focalDistance = vec.length();

  // now, move in 20% closer to the focal point...
#define ZOOM_FACTOR 20

  camera->position = camera->position.getValue() +
    (vec * (float(ZOOM_FACTOR) / 100.0f));
  vec = point - camera->position.getValue();
  camera->focalDistance = vec.length();

  PRIVATE(this)->rpaction->reset();
  return FALSE;
}

#undef ZOOM_FACTOR

#undef PRIVATE

// *************************************************************************

NbCenterModeP::NbCenterModeP(NbCenterMode * api)
{
  this->rpaction = NULL;
}
