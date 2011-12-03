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

#include <Inventor/lists/SbList.h>
#include <Inventor/events/SoEvent.h>

#include <NutsnBolts/navigation/NbNavigationState.h>
#include <NutsnBolts/misc/SoEvent.h>

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
  if (trigger) {
    PRIVATE(this)->triggerstack.append(SoEvent_Clone(trigger));
  } else {
    PRIVATE(this)->triggerstack.append(NULL);
  }
}

/*!
  Pops the mode state stack.
*/

void
NbNavigationState::pop(void)
{
  PRIVATE(this)->modestack.pop();
  const SoEvent * trigger = PRIVATE(this)->triggerstack.pop();
  if (trigger != NULL ) {
    delete trigger;
  }
}

/*!
  Resets the mode state stack.
*/

void
NbNavigationState::reset(void)
{
  PRIVATE(this)->modestack.truncate(0);
  int i;
  for ( i = 0; i < PRIVATE(this)->triggerstack.getLength(); i++ ) {
    const SoEvent * trigger = PRIVATE(this)->triggerstack[i];
    if (trigger != NULL) {
      delete trigger;
    }
  }
  PRIVATE(this)->triggerstack.truncate(0);
}

/*!
  Returns the mode at the top of the mode state stack.

  \sa push, pop, getTrigger
*/

NbNavigationMode *
NbNavigationState::getMode(void) const
{
  if (PRIVATE(this)->modestack.getLength() == 0) return NULL;
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
  if (PRIVATE(this)->modestack.getLength() == 0) return NULL;
  return PRIVATE(this)->triggerstack[PRIVATE(this)->triggerstack.getLength()-1];
}

#undef PRIVATE

// *************************************************************************

NbNavigationStateP::NbNavigationStateP(NbNavigationState * api)
{
}

