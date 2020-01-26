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

#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec3d.h>
#include <Inventor/SbPlane.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoCamera.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>

#include <NutsnBolts/navigation/NbOrthoCenterMode.h>

// *************************************************************************

/*!
  \class NbCenterMode NutsnBolts/navigation/NbCenterMode.h
  \brief Class that implements a focal point centering mode.

  This class implements the focal point centering mode, moving the camera
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

  SbVec3d pickpos;
  SoPath * path = ctrl->pick(event->getPosition(), pickpos);

  if (path != NULL) {
    SbRotation rot = camera->orientation.getValue();
    SbVec3f up(0, 1, 0);
    SbVec3f right(1, 0, 0);
    SbVec3f pos = camera->position.getValue();
    rot.multVec(up, up);
    rot.multVec(right, right);

    SbPlane plane(pos, pos+up, pos+right);

    SbVec3f pickposf((float)pickpos[0], (float)pickpos[1], (float)pickpos[2]);
    float distance = plane.getDistance(pickposf);
    SbVec3f normal = plane.getNormal();
    normal.normalize();

    SbVec3f newpos = pickposf - (normal * distance);
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
