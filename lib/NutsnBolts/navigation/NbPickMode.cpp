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

#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/SbPList.h>

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
  This method handles the pitching motion of the camera, based on user input.
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
      SbVec3f pos;
      SoPath * path = ctrl->pick(event->getPosition(), pos);
      if ( path ) {
        this->invokePickCallbacks(pos, path);
      } else {
        this->invokePickCallbacks(SbVec3f(0, 0, 0), NULL);
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
NbPickMode::invokePickCallbacks(const SbVec3f & pos, const SoPath * path)
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
