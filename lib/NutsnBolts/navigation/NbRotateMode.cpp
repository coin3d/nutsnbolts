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

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>

#include <NutsnBolts/navigation/NbRotateMode.h>

// *************************************************************************

/*!
  \class NbRotateMode NutsnBolts/navigation/NbRotateMode.h
  \brief Class that implements the rotate mode.

  This class implements the object-rotate mode, rotating the camera
  around the focal point.

  \ingroup navigation
*/

class NbRotateModeP {
public:
  NbRotateModeP(NbRotateMode * api) {
    this->projector = NULL;
  }
  ~NbRotateModeP(void) {
    if ( this->projector ) {
      delete this->projector;
      this->projector = NULL;
    }
  }

public:
  SbSphereSheetProjector * projector;

}; // NbRotateModeP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/


NbRotateMode::NbRotateMode(SbName modename)
  : inherited(modename)
{
  PRIVATE(this) = new NbRotateModeP(this);
}

/*!
  Destructor.
*/

NbRotateMode::~NbRotateMode(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  This method implements the object rotatio through tracking how the mouse
  location moves on a spheresheet-projector.

  Returns FALSE for unused events and TRUE for events that are used.
*/

SbBool
NbRotateMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if ( !event->isOfType(SoLocation2Event::getClassTypeId()) ) {
    return FALSE;
  }

  SbVec2f initpos(this->getInitialNormalizedPosition(ctrl));
  SbVec2f currentpos(this->getCurrentNormalizedPosition(ctrl));

  if ( PRIVATE(this)->projector == NULL ) {
    PRIVATE(this)->projector =
      new SbSphereSheetProjector(SbSphere(SbVec3f(0, 0, 0), 0.8f));
    SbViewVolume volume;
    volume.ortho(-1, 1, -1, 1, -1, 1);
    PRIVATE(this)->projector->setViewVolume(volume);
  }

  ctrl->restoreCamera();

  PRIVATE(this)->projector->project(initpos);
  SbRotation rot;
  PRIVATE(this)->projector->projectAndGetRotation(currentpos, rot);
  rot.invert();

  ctrl->reorientCamera(rot);
  return TRUE;
}

#undef PRIVATE
