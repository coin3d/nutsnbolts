/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <stdio.h>
#include <assert.h>

#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/nodes/SoCamera.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>

#include <NutsnBolts/navigation/NbPanMode.h>

// *************************************************************************

/*!
  \class NbPanMode NutsnBolts/navigation/NbPanMode.h
  \brief Class that implements the panning mode.

  This class implements the panning mode.

  \ingroup navigation
*/

NB_NAVIGATION_MODE_SOURCE(NbPanMode);

void
NbPanMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbPanMode, NbNavigationMode);
}

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

NbNavigationMode *
NbPanMode::clone(void) const
{
  return new NbPanMode(this->getModeName());
}

/*!
  This method implements the panning operation.

  Returns FALSE for events that are not used, and TRUE for events that
  are used.
*/

SbBool
NbPanMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }

  SoCamera * camera = ctrl->getCamera();
  if (!camera) {
    return FALSE;
  }

  // Find projection points for the current and the reference mouse
  // coordinates.
  SbViewVolume vv = camera->getViewVolume(ctrl->getViewportAspect());
  SbPlane panningplane = vv.getPlane(camera->focalDistance.getValue());
  SbLine line;
  SbVec3f current_planept, initial_planept;
  vv.projectPointToLine(this->getCurrentNormalizedPosition(ctrl), line);
  panningplane.intersect(line, current_planept);
  vv.projectPointToLine(this->getInitialNormalizedPosition(ctrl), line);
  panningplane.intersect(line, initial_planept);

  ctrl->restoreCamera();
  ctrl->moveCamera(initial_planept - current_planept);

  return TRUE;
}
