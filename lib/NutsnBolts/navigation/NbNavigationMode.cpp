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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Inventor/SbVec2s.h>
#include <Inventor/SbDict.h>
#include <Inventor/SbPList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/nodes/SoCamera.h>

#include <NutsnBolts/misc/SoEvent.h>

#include <NutsnBolts/navigation/NbNavigationMode.h>
#include <NutsnBolts/navigation/NbNavigationInfo.h>

#include <NutsnBolts/navigation/NbIdleMode.h>
#include <NutsnBolts/navigation/NbRotateMode.h>
#include <NutsnBolts/navigation/NbPanMode.h>
#include <NutsnBolts/navigation/NbZoomMode.h>

// *************************************************************************

/*!
  \class NbNavigationMode NutsnBolts/navigation/NbNavigationMode.h
  \brief Abstract base class for implementing a navigation submode.

  This class is internal and should mostly be of interest for developers
  planning on implementing their own navigation systems with new navigation
  submodes.

  It is abstract.  The handleEvent method needs to be reimplemented
  by subclasses.

  \ingroup navigation
*/

// *************************************************************************

struct Transition {
  const SoEvent * trigger;
  const SoEvent * condition;
  NbNavigationMode * mode;
  SbBool stack;
};

class NbNavigationModeP {
public:
  NbNavigationModeP(void);

  SbName modename;

  SbVec2s initpos;
  SbVec2s prevpos;
  SbVec2s currentpos;
}; // NbNavigationModeP

// *************************************************************************

/*!
  Static class initializer.
*/

void
NbNavigationMode::initClass(void)
{
}

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.  The \a modename argument is the name of the mode.
  It is often useful to create multiple instances of the same mode
  but with different names in a navigation system.
*/

NbNavigationMode::NbNavigationMode(SbName modename)
{
  PRIVATE(this) = new NbNavigationModeP;
  PRIVATE(this)->modename = modename;
}

/*!
  Destructor.
*/

NbNavigationMode::~NbNavigationMode(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Returns the name of the mode, given to the constructor.
*/

SbName
NbNavigationMode::getModeName(void) const
{
  return PRIVATE(this)->modename;
}

/*!
  This is the entry point for events coming from the
  NbNavigationSystem.  Some initial information is gathered before the
  event is passed on to the virtual, abstract handleEvent method.

  \sa handleEvent
*/

SbBool
NbNavigationMode::processEvent(const SoEvent * event, const NbNavigationInfo * info)
{
  if ( event->isOfType(SoLocation2Event::getClassTypeId()) )
    PRIVATE(this)->prevpos = PRIVATE(this)->currentpos;

  PRIVATE(this)->currentpos = event->getPosition();

  return this->handleEvent(event, info);
}

/*!
  Not implemented yet.

  This function should return a visualization scene graph in case the
  navigation submode also includes visualization hints for the 3D
  display.
*/

SoNode *
NbNavigationMode::getSceneGraph(void)
{
  return NULL;
}

/*!
  \fn SbBool NbNavigationMode::handleEvent(const SoEvent * event, const NbNavigationInfo * into) = 0

  Virtual abstract method that should be overridden to implement the
  navigation submode.

  All events passed through the system while this mode is active is
  passed to this method, including the events that cause init(),
  abort(), and finish() event transitions.

*/

/*!
  Invoked when a mode is started/initialized.  It is passed the event that
  triggered the mode change, or NULL in the case of being the default mode.

  This method is called just before handleEvent is called with the
  exact same event object.

  If overriding this event, control should still be passed down to this
  method.

  \sa handleEvent, abort(), finish()
*/

void
NbNavigationMode::init(const SoEvent * event, const NbNavigationInfo * info)
{
  // fprintf(stderr, "NbNavigationMode::init() %s\n",
  // 	  this->getModeName().getString());
  PRIVATE(this)->initpos = event->getPosition();
  PRIVATE(this)->prevpos = event->getPosition();
  PRIVATE(this)->currentpos = event->getPosition();
}

/*!
  This method is invoked when themode is aborted.  The event passed to
  this method has already been handed to handleEvent as well.

  \sa handleEvent, init(), finish()
*/

void
NbNavigationMode::abort(const SoEvent * event, const NbNavigationInfo * info)
{
  // fprintf(stderr, "NbNavigationMode::abort() %s\n",
  // 	  this->getModeName().getString());
  info->restoreCamera();
}

/*!
  Invoked when a mode is completed.  The event object passed to this method
  has already been handed to handleEvent as well.

  \sa handleEvent, init(), abort()
*/

void
NbNavigationMode::finish(const SoEvent * event, const NbNavigationInfo * info)
{
  // fprintf(stderr, "NbNavigationMode::finish() %s\n",
  // 	  this->getModeName().getString());
  info->setCamera();
}

// *************************************************************************

/*!
  Returns the initial pointer position in the viewport, from when the
  navigation submode was started.

  \sa getPreviousPosition, getCurrentPosition, getInitialNormalizedPosition
*/

SbVec2s
NbNavigationMode::getInitialPosition(void) const
{
  return PRIVATE(this)->initpos;
}

/*!
  Returns the previous points position in the viewport.

  \sa getInitialPosition, getCurrentPosition, getPreviousNormalizedPosition
*/

SbVec2s
NbNavigationMode::getPreviousPosition(void) const
{
  return PRIVATE(this)->prevpos;
}

/*!
  Returns the current pointer position in the viewport.

  \sa getInitialPosition, getPreviousPosition, getCurrentNormalizedPosition
*/

SbVec2s
NbNavigationMode::getCurrentPosition(void) const
{
  return PRIVATE(this)->currentpos;
}

/*!
  Returns the initial pointer position in normalized coordinates.

  \sa getInitialPosition, getPreviousNormalizedPosition, getCurrentNormalizedPosition
*/

SbVec2f
NbNavigationMode::getInitialNormalizedPosition(const NbNavigationInfo * info) const
{
  SbVec2s vp(info->getViewportSize());
  SbVec2s pos(this->getInitialPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

/*!
  Returns the previuos pointer position in normalized coordinates.

  \sa getPreviousPosition, getInitialNormalizedPosition, getCurrentNormalizedPosition
*/

SbVec2f 
NbNavigationMode::getPreviousNormalizedPosition(const NbNavigationInfo * info) const
{
  SbVec2s vp(info->getViewportSize());
  SbVec2s pos(this->getPreviousPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

/*!
  Returns the current pointer position in normalized coordinates.

  \sa getCurrentPosition, getInitialNormalizedPosition, getPreviousNormalizedPosition
*/


SbVec2f
NbNavigationMode::getCurrentNormalizedPosition(const NbNavigationInfo * info) const
{
  SbVec2s vp(info->getViewportSize());
  SbVec2s pos(this->getCurrentPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

#undef PRIVATE

// *************************************************************************
// private class implementation

NbNavigationModeP::NbNavigationModeP(void)
{
  this->initpos.setValue(0, 0);
  this->prevpos.setValue(0, 0);
  this->currentpos.setValue(0, 0);
}
