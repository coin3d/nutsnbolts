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
#include <Inventor/nodes/SoCamera.h>

#include <NutsnBolts/navigation/NbNavigationInfo.h>

#include <NutsnBolts/navigation/NbPanMode.h>

// *************************************************************************

/*!
  \class NbPanMode NutsnBolts/navigation/NbPanMode.h
  \brief Class that implements the panning mode.

  This class implements the panning mode.

  \ingroup navigation
*/

/*!
  Constructor.
*/

NbPanMode::NbPanMode(SbName name)
  : inherited(name)
{
  // we don't have the need for a private implementation, but we have
  // set off space for one if we ever need one in the future.
  this->pimpl = NULL;
}

/*!
  Destructor.
*/

NbPanMode::~NbPanMode(void)
{
}

/*!
  This method implements the panning operation.

  Returns FALSE for events that are not used, and TRUE for events that
  are used.
*/

SbBool
NbPanMode::handleEvent(const SoEvent * event, const NbNavigationInfo * info)
{
  if ( ! event->isOfType(SoLocation2Event::getClassTypeId()) ) {
    return FALSE;
  }

  SoCamera * camera = info->getCamera();
  if ( !camera ) {
    return FALSE;
  }

  // Find projection points for the current and the reference mouse
  // coordinates.
  SbViewVolume vv = camera->getViewVolume(info->getViewportAspect());
  SbPlane panningplane = vv.getPlane(camera->focalDistance.getValue());
  SbLine line;
  SbVec3f current_planept, initial_planept;
  vv.projectPointToLine(this->getCurrentNormalizedPosition(info), line);
  panningplane.intersect(line, current_planept);
  vv.projectPointToLine(this->getInitialNormalizedPosition(info), line);
  panningplane.intersect(line, initial_planept);

  info->restoreCamera();
  info->moveCamera(initial_planept - current_planept);

  return TRUE;
}
