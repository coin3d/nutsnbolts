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

#include <Inventor/events/SoLocation2Event.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>
#include <NutsnBolts/navigation/NbPitchMode.h>

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbPitchMode);

void
NbPitchMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbPitchMode, NbNavigationMode);
}

/*!
  Constructor.
*/

NbPitchMode::NbPitchMode(SbName name)
: inherited(name)
{
  PRIVATE(this) = NULL;
  this->set1DValueFunc(NbNavigationMode::getMouseMoveVerticalNormalizedDistance, NULL);
}

/*!
  Destructor.
*/

NbPitchMode::~NbPitchMode(void)
{
}

NbNavigationMode *
NbPitchMode::clone(void) const
{
  return new NbPitchMode(this->getModeName());
}

/*!
  This method handles the pitching motion of the camera, based on user input.
*/

SbBool
NbPitchMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }
  ctrl->restoreCamera();
  float value = this->get1DValue(ctrl);
  ctrl->pitchCamera(value);
  return TRUE;
}

#undef PRIVATE
