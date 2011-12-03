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
    if (this->projector) {
      delete this->projector;
      this->projector = NULL;
    }
  }

public:
  SbSphereSheetProjector * projector;

}; // NbRotateModeP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbRotateMode);

void
NbRotateMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbRotateMode, NbNavigationMode);
}

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

NbNavigationMode *
NbRotateMode::clone(void) const
{
  return new NbRotateMode(this->getModeName());
}

/*!
  This method implements the object rotatio through tracking how the mouse
  location moves on a spheresheet-projector.

  Returns FALSE for unused events and TRUE for events that are used.
*/

SbBool
NbRotateMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (!event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }

  SbVec2f initpos(this->getInitialNormalizedPosition(ctrl));
  SbVec2f currentpos(this->getCurrentNormalizedPosition(ctrl));

  if (PRIVATE(this)->projector == NULL) {
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
