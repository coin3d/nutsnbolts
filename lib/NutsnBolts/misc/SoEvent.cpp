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

#include <Inventor/events/SoEvents.h>

#include <NutsnBolts/misc/SoEvent.h>

// *************************************************************************

/*!
  \file NutsnBolts/misc/SoEvent.h
  This file implements a couple of functions related to SoEvent objects.

  Internal file.

  \ingroup misc
  \ingroup internal
*/

/*!
  Returns TRUE if \a ev1 and \a ev2 are equal events, and FALSE if they
  differ.

  Only which buttons and which states the events have are checked.
  Modifier states, time stamps, and screen positions are ignored.
*/

SbBool
SoEvent_Equals(const SoEvent * ev1, const SoEvent * ev2)
{
  assert(ev1 != NULL);
  assert(ev2 != NULL);
  if (ev1->getTypeId() != ev2->getTypeId()) {
    return FALSE;
  }
  if (ev1->getTypeId() == SoMouseButtonEvent::getClassTypeId()) {
    SoMouseButtonEvent * mb1 = (SoMouseButtonEvent *) ev1;
    SoMouseButtonEvent * mb2 = (SoMouseButtonEvent *) ev2;
    if (mb1->getState() == mb2->getState() &&
        mb1->getButton() == mb2->getButton()) {
      return TRUE;
    }
  } else if (ev1->getTypeId() == SoKeyboardEvent::getClassTypeId()) {
    SoKeyboardEvent * kb1 = (SoKeyboardEvent *) ev1;
    SoKeyboardEvent * kb2 = (SoKeyboardEvent *) ev2;
    if (kb1->getState() == kb2->getState() &&
        kb1->getKey() == kb2->getKey()) {
      return TRUE;
    }
  }
  return FALSE;
}

/*!
  This function clones an event.

  Only which buttons and states the event has is copied.  Modifier
  states, time stamps, and window positions are ignored.
*/

SoEvent *
SoEvent_Clone(const SoEvent * event)
{
  assert(event != NULL);
  SoType type = event->getTypeId();
  SoEvent * clone = NULL;
  if (type == SoKeyboardEvent::getClassTypeId()) {
    SoKeyboardEvent * kbevent = new SoKeyboardEvent;
    SoKeyboardEvent * ev = (SoKeyboardEvent *) event;
    kbevent->setState(ev->getState());
    kbevent->setKey(ev->getKey());
    clone = kbevent;
  } else if (type == SoMouseButtonEvent::getClassTypeId()) {
    SoMouseButtonEvent * mbevent = new SoMouseButtonEvent;
    SoMouseButtonEvent * ev = (SoMouseButtonEvent *) event;
    mbevent->setState(ev->getState());
    mbevent->setButton(ev->getButton());
    clone = mbevent;
  }
  return clone;
}

/*!
  Dump debug information about event \a ev to stream \a stream.
*/

void
SoEvent_Dump(FILE * stream, const SoEvent * ev)
{
  SoType type = ev->getTypeId();
  fprintf(stream, "Event type: %s\n", type.getName().getString());
  if (type == SoMouseButtonEvent::getClassTypeId()) {
    SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) ev;
    fprintf(stream, "  Button State: ");
    switch (mbevent->getState()) {
    case SoButtonEvent::DOWN:
      fprintf(stream, "DOWN\n");
      break;
    case SoButtonEvent::UP:
      fprintf(stream, "UP\n");
      break;
    default:
      fprintf(stream, "<unknown>\n");
      break;
    }
    fprintf(stream, "  Button: ");
    switch (mbevent->getButton()) {
    case SoMouseButtonEvent::BUTTON1:
      fprintf(stream, "button 1\n");
      break;
    case SoMouseButtonEvent::BUTTON2:
      fprintf(stream, "button 2\n");
      break;
    case SoMouseButtonEvent::BUTTON3:
      fprintf(stream, "button 3\n");
      break;
    case SoMouseButtonEvent::BUTTON4:
      fprintf(stream, "button 4\n");
      break;
    case SoMouseButtonEvent::BUTTON5:
      fprintf(stream, "button 5\n");
      break;
    default:
      fprintf(stream, "<unknown>\n");
      break;
    }
  }
}
