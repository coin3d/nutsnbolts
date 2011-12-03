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
