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
#include <NutsnBolts/navigation/NbNavigationControl.h>

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

/*
  FIXME: the internal mode state data is still flawed since the same mode
  object can potentially go on the stack multiple times. Need to only put
  clones on the stack or something, because data must be initialized on
  push, not on original object creation, and each stack-level needs
  its own data.
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

  NbNavigation1DInputValueFunc * valuefunc1d;
  NbNavigation2DInputValueFunc * valuefunc2d;
  NbNavigation3DInputValueFunc * valuefunc3d;
  void * valuefuncclosure;

  SbBool aborted;
  SbBool finished;
}; // NbNavigationModeP

// *************************************************************************

NB_NAVIGATION_MODE_SOURCE(NbNavigationMode);

/*!
  Static class initializer.
*/

void
NbNavigationMode::initClass(void)
{
  assert(NbNavigationMode::classTypeId == SoType::badType() &&
         "don't init() twice!");
  /* Set up entry in the type system. */
  NbNavigationMode::classTypeId = SoType::createType(SoType::badType(),
                                                     "NbNavigationMode",
                                                     NULL /* createfunc */,
                                                     0 /* data */);
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
  \sa get1DValueFunc, set2DValueFunc, set3DValueFunc
*/

void
NbNavigationMode::set1DValueFunc(NbNavigation1DInputValueFunc * func, void * closure)
{
  PRIVATE(this)->valuefunc1d = func;
  PRIVATE(this)->valuefunc2d = NULL;
  PRIVATE(this)->valuefunc3d = NULL;
  PRIVATE(this)->valuefuncclosure = closure;
}

void
NbNavigationMode::set2DValueFunc(NbNavigation2DInputValueFunc * func, void * closure)
{
  PRIVATE(this)->valuefunc1d = NULL;
  PRIVATE(this)->valuefunc2d = func;
  PRIVATE(this)->valuefunc3d = NULL;
  PRIVATE(this)->valuefuncclosure = closure;
}

void
NbNavigationMode::set3DValueFunc(NbNavigation3DInputValueFunc * func, void * closure)
{
  PRIVATE(this)->valuefunc1d = NULL;
  PRIVATE(this)->valuefunc2d = NULL;
  PRIVATE(this)->valuefunc3d = func;
  PRIVATE(this)->valuefuncclosure = closure;
}

/*!
  \sa set1DValueFunc, get2DValueFunc, get3DValueFunc
*/

NbNavigation1DInputValueFunc *
NbNavigationMode::get1DValueFunc(void ** closureptr) const
{
  if (!PRIVATE(this)->valuefunc1d) {
    return NULL;
  }
  if (closureptr != NULL) {
    *closureptr = PRIVATE(this)->valuefuncclosure;
  }
  return PRIVATE(this)->valuefunc1d;
}

NbNavigation2DInputValueFunc *
NbNavigationMode::get2DValueFunc(void ** closureptr) const
{
  if (!PRIVATE(this)->valuefunc2d) {
    return NULL;
  }
  if (closureptr != NULL) {
    *closureptr = PRIVATE(this)->valuefuncclosure;
  }
  return PRIVATE(this)->valuefunc2d;
}

NbNavigation3DInputValueFunc *
NbNavigationMode::get3DValueFunc(void ** closureptr) const
{
  if (!PRIVATE(this)->valuefunc3d) {
    return NULL;
  }
  if (closureptr != NULL) {
    *closureptr = PRIVATE(this)->valuefuncclosure;
  }
  return PRIVATE(this)->valuefunc3d;
}

/*!
  This is the entry point for events coming from the
  NbNavigationSystem.  Some initial information is gathered before the
  event is passed on to the virtual, abstract handleEvent method.

  \sa handleEvent
*/

SbBool
NbNavigationMode::processEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{
  if (event->isOfType(SoLocation2Event::getClassTypeId()))
    PRIVATE(this)->prevpos = PRIVATE(this)->currentpos;

  PRIVATE(this)->currentpos = event->getPosition();

  return this->handleEvent(event, ctrl);
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
  \fn SbBool NbNavigationMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl) = 0

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
NbNavigationMode::init(const SoEvent * event, const NbNavigationControl * ctrl)
{
  PRIVATE(this)->initpos = event->getPosition();
  PRIVATE(this)->prevpos = event->getPosition();
  PRIVATE(this)->currentpos = event->getPosition();
  PRIVATE(this)->aborted = FALSE;
  PRIVATE(this)->finished = FALSE;
}

/*!
  This method is invoked when themode is aborted.  The event passed to
  this method has already been handed to handleEvent as well.

  \sa handleEvent, init(), finish()
*/

void
NbNavigationMode::abort(const SoEvent * event, const NbNavigationControl * ctrl)
{
  ctrl->restoreCamera();
}

/*!
  Invoked when a mode is completed.  The event object passed to this method
  has already been handed to handleEvent as well.

  \sa handleEvent, init(), abort()
*/

void
NbNavigationMode::finish(const SoEvent * event, const NbNavigationControl * ctrl)
{
  ctrl->saveCamera();
}

// *************************************************************************

float
NbNavigationMode::get1DValue(const NbNavigationControl * ctrl) const
{
  assert(PRIVATE(this)->valuefunc1d);
  return PRIVATE(this)->valuefunc1d(PRIVATE(this)->valuefuncclosure, this, ctrl);
}

SbVec2f
NbNavigationMode::get2DValue(const NbNavigationControl * ctrl) const
{
  assert(PRIVATE(this)->valuefunc2d);
  return PRIVATE(this)->valuefunc2d(PRIVATE(this)->valuefuncclosure, this, ctrl);
}

SbVec3f
NbNavigationMode::get3DValue(const NbNavigationControl * ctrl) const
{
  assert(PRIVATE(this)->valuefunc3d);
  return PRIVATE(this)->valuefunc3d(PRIVATE(this)->valuefuncclosure, this, ctrl);
}

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
NbNavigationMode::getInitialNormalizedPosition(const NbNavigationControl * ctrl) const
{
  SbVec2s vp(ctrl->getViewportSize());
  SbVec2s pos(this->getInitialPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

/*!
  Returns the previuos pointer position in normalized coordinates.

  \sa getPreviousPosition, getInitialNormalizedPosition, getCurrentNormalizedPosition
*/

SbVec2f 
NbNavigationMode::getPreviousNormalizedPosition(const NbNavigationControl * ctrl) const
{
  SbVec2s vp(ctrl->getViewportSize());
  SbVec2s pos(this->getPreviousPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

/*!
  Returns the current pointer position in normalized coordinates.

  \sa getCurrentPosition, getInitialNormalizedPosition, getPreviousNormalizedPosition
*/


SbVec2f
NbNavigationMode::getCurrentNormalizedPosition(const NbNavigationControl * ctrl) const
{
  SbVec2s vp(ctrl->getViewportSize());
  SbVec2s pos(this->getCurrentPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

// *************************************************************************

float
NbNavigationMode::getAbsMouseMovePixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  SbVec2f vec((float)(currentpos[0] - initpos[0]), 
              (float)(currentpos[1] - initpos[1]));
  return vec.length();
}

float
NbNavigationMode::getMouseMoveVerticalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  return float(currentpos[1] - initpos[1]);
}

float
NbNavigationMode::getMouseMoveVerticalNormalizedDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  SbVec2s vp = ctrl->getViewportSize();
  return float(currentpos[1] - initpos[1]) / float(vp[1]);
}

float
NbNavigationMode::getMouseMoveHorizontalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  return float(currentpos[0] - initpos[0]);
}

float
NbNavigationMode::getMouseMoveHorizontalNormalizedDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  SbVec2s initpos = mode->getInitialPosition();
  SbVec2s currentpos = mode->getCurrentPosition();
  SbVec2s vp = ctrl->getViewportSize();
  return float(currentpos[0] - initpos[0]) / float(vp[0]);
}

float
NbNavigationMode::getAbsMouseCenterPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  return 0.0f;
}

float
NbNavigationMode::getMouseCenterVerticalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  return 0.0f;
}

float
NbNavigationMode::getMouseCenterHorizontalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  return 0.0f;
}

float
NbNavigationMode::getMouseMoveAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  return 0.0f;
}

float
NbNavigationMode::getMouseCenterAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  return 0.0f;
}

float
NbNavigationMode::getMouseMoveCenterAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl)
{
  return 0.0f;
}

void
NbNavigationMode::abort(void)
{
  if (this->isFinished()) return;
  PRIVATE(this)->aborted = TRUE;
}

void
NbNavigationMode::finish(void)
{
  if (this->isAborted()) return;
  PRIVATE(this)->finished = TRUE;
}

SbBool
NbNavigationMode::isAborted(void) const
{
  return PRIVATE(this)->aborted;
}

SbBool
NbNavigationMode::isFinished(void) const
{
  return PRIVATE(this)->finished;
}

#undef PRIVATE

// *************************************************************************
// private class implementation

NbNavigationModeP::NbNavigationModeP(void)
{
  this->initpos.setValue(0, 0);
  this->prevpos.setValue(0, 0);
  this->currentpos.setValue(0, 0);
  this->valuefunc1d = NULL;
  this->valuefunc2d = NULL;
  this->valuefunc3d = NULL;
  this->valuefuncclosure = NULL;
  this->aborted = FALSE;
  this->finished = FALSE;
}
