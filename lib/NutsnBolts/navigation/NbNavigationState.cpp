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

#include <Inventor/lists/SbList.h>

#include <NutsnBolts/navigation/NbNavigationState.h>

// *************************************************************************

/*!
  \class NbNavigationState NutsnBolts/navigation/NbNavigationState.h
  \brief Internal class.

  This class is only used internally in the NbNavigationSystem class
  to track the current navigation state (current mode).  It isn't
  exposed anywhere.

  \ingroup navigation
*/

// *************************************************************************

class NbNavigationStateP {
public:
  NbNavigationStateP(NbNavigationState * api);

  SbList<NbNavigationMode *> modestack;
  SbList<const SoEvent *> triggerstack;
};

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/

NbNavigationState::NbNavigationState(void)
{
  PRIVATE(this) = new NbNavigationStateP(this);
}
/*!
  Destructor.
*/

NbNavigationState::~NbNavigationState(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Pushes a new mode on the state stack.  \a trigger is the event that
  triggered the mode change.

  \sa pop, getMode, getTrigger
*/

void
NbNavigationState::push(NbNavigationMode * mode, const SoEvent * trigger)
{
  PRIVATE(this)->modestack.append(mode);
  PRIVATE(this)->triggerstack.append(trigger);
}

/*!
  Pops the mode state stack.
*/

void
NbNavigationState::pop(void)
{
  PRIVATE(this)->modestack.pop();
  PRIVATE(this)->triggerstack.pop();
}

/*!
  Resets the mode state stack.
*/

void
NbNavigationState::reset(void)
{
  PRIVATE(this)->modestack.truncate(0);
  PRIVATE(this)->triggerstack.truncate(0);
}

/*!
  Returns the mode at the top of the mode state stack.

  \sa push, pop, getTrigger
*/

NbNavigationMode *
NbNavigationState::getMode(void) const
{
  if ( PRIVATE(this)->modestack.getLength() == 0 ) return NULL;
  return PRIVATE(this)->modestack[PRIVATE(this)->modestack.getLength()-1];
}

/*!
  Returns the event that triggered the current mode.

  NULL is returned in the case of the initial mode and if the mode
  state stack has been popped empty.

  \sa getMode, push, pop
*/

const SoEvent *
NbNavigationState::getTrigger(void) const
{
  if ( PRIVATE(this)->modestack.getLength() == 0 ) return NULL;
  return PRIVATE(this)->triggerstack[PRIVATE(this)->triggerstack.getLength()-1];
}

#undef PRIVATE

// *************************************************************************

NbNavigationStateP::NbNavigationStateP(NbNavigationState * api)
{
}

