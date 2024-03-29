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

#include <cassert>
#include <cstdio>

#include <Inventor/events/SoLocation2Event.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>
#include <NutsnBolts/navigation/NbRollMode.h>

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbRollMode);

void
NbRollMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbRollMode, NbNavigationMode);
}

NbRollMode::NbRollMode(SbName name)
: inherited(name)
{
  PRIVATE(this) = NULL;
  this->set1DValueFunc(NbNavigationMode::getMouseMoveCenterAngle, NULL);
}

NbRollMode::~NbRollMode(void)
{
}

NbNavigationMode *
NbRollMode::clone(void) const
{
  return new NbRollMode(this->getModeName());
}

SbBool
NbRollMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }

  ctrl->restoreCamera();
  float angle = this->get1DValue(ctrl);
  ctrl->rollCamera(angle);
  return TRUE;
}

#undef PRIVATE
