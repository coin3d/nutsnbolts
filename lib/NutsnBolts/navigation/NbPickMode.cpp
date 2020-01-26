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

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/SbPList.h>
#include <Inventor/SbVec3d.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>
#include <NutsnBolts/navigation/NbPickMode.h>

// *************************************************************************

class NbPickModeP {
public:
  NbPickModeP(NbPickMode * api);
  ~NbPickModeP(void);

  SbPList callbacks;
}; // NbPickModeP

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbPickMode);

void
NbPickMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbPickMode, NbNavigationMode);
}

/*!
  Constructor.
*/

NbPickMode::NbPickMode(SbName name)
: inherited(name)
{
  PRIVATE(this) = new NbPickModeP(this);
}

/*!
  Destructor.
*/

NbPickMode::~NbPickMode(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

NbNavigationMode *
NbPickMode::clone(void) const
{
  return new NbPickMode(this->getModeName());
}

/*!
  This method handles the picking events based on user input.
*/

SbBool
NbPickMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (!event->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    return FALSE;
  }
  SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) event;
  if (mbevent->getButton() == SoMouseButtonEvent::BUTTON1) {
    if (mbevent->getState() == SoButtonEvent::DOWN) {
      SbVec3d pos;
      SoPath * path = ctrl->pick(event->getPosition(), pos);
      if ( path ) {
        this->invokePickCallbacks(pos, path);
      } else {
        this->invokePickCallbacks(SbVec3d(0, 0, 0), NULL);
      }
    } else {
      // button up - do nothing, but don't pass event on
    }
    return TRUE;
  }
  return FALSE;
}

void
NbPickMode::addPickCallback(NbPickModePickCB * cb, void * closure)
{
  PRIVATE(this)->callbacks.append((void *) cb);
  PRIVATE(this)->callbacks.append(closure);
}

void
NbPickMode::removePickCallback(NbPickModePickCB * cb, void * closure)
{
  const int max = PRIVATE(this)->callbacks.getLength();
  int i;
  for ( i = 0; i < max; i += 2 ) {
    NbPickModePickCB * callback = (NbPickModePickCB *)
      PRIVATE(this)->callbacks[i];
    void * data = PRIVATE(this)->callbacks[i+1];
    if (callback == cb && data == closure) {
      PRIVATE(this)->callbacks.remove(i+1);
      PRIVATE(this)->callbacks.remove(i);
      return;
    }
  }
}

void
NbPickMode::invokePickCallbacks(const SbVec3d & pos, const SoPath * path)
{
  const int max = PRIVATE(this)->callbacks.getLength();
  int i;
  for ( i = 0; i < max; i += 2 ) {
    NbPickModePickCB * callback = (NbPickModePickCB *)
      PRIVATE(this)->callbacks[i];
    void * closure = PRIVATE(this)->callbacks[i+1];
    callback(closure, pos, path);
  }
}

#undef PRIVATE

// *************************************************************************

NbPickModeP::NbPickModeP(NbPickMode * api)
{
}

NbPickModeP::~NbPickModeP(void)
{
}

// *************************************************************************
