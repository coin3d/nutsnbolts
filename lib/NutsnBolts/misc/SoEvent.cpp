
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
  if ( ev1->getTypeId() != ev2->getTypeId() ) {
    return FALSE;
  }
  if ( ev1->getTypeId() == SoMouseButtonEvent::getClassTypeId() ) {
    SoMouseButtonEvent * mb1 = (SoMouseButtonEvent *) ev1;
    SoMouseButtonEvent * mb2 = (SoMouseButtonEvent *) ev2;
    if ( mb1->getState() == mb2->getState() &&
	 mb1->getButton() == mb2->getButton() ) {
      return TRUE;
    }
  } else if ( ev1->getTypeId() == SoKeyboardEvent::getClassTypeId() ) {
    SoKeyboardEvent * kb1 = (SoKeyboardEvent *) ev1;
    SoKeyboardEvent * kb2 = (SoKeyboardEvent *) ev2;
    if ( kb1->getState() == kb2->getState() &&
	 kb1->getKey() == kb2->getKey() ) {
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
  if ( type == SoKeyboardEvent::getClassTypeId() ) {
    SoKeyboardEvent * kbevent = new SoKeyboardEvent;
    SoKeyboardEvent * ev = (SoKeyboardEvent *) event;
    kbevent->setState(ev->getState());
    kbevent->setKey(ev->getKey());
    clone = kbevent;
  } else if ( type == SoMouseButtonEvent::getClassTypeId() ) {
    SoMouseButtonEvent * mbevent = new SoMouseButtonEvent;
    SoMouseButtonEvent * ev = (SoMouseButtonEvent *) event;
    mbevent->setState(ev->getState());
    mbevent->setButton(ev->getButton());
    clone = mbevent;
  }
  return clone;
}
