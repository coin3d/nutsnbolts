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
  // static factory stuff
  static SbDict * namedict;

public:
  NbNavigationModeP(void);

  SbName modename;

  SbBool active;
  NbNavigationMode * submode;
  const SoEvent * trigger;

  SbVec2s initpos;
  SbVec2s prevpos;
  SbVec2s currentpos;

  SbList<Transition> * transitions;
  SbList<Transition> * finishes;
  SbList<Transition> * aborts;

}; // NbNavigationModeP

SbDict * NbNavigationModeP::namedict = NULL;

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
  // this->setCamera(NULL);

  if ( PRIVATE(this)->trigger ) {
    delete PRIVATE(this)->trigger;
    PRIVATE(this)->trigger = NULL;
  }

  if ( PRIVATE(this)->transitions ) {
    int i = 0;
    int max = PRIVATE(this)->transitions->getLength();
    for ( i = 0; i < max; i++ ) {
      Transition t = (*(PRIVATE(this)->transitions))[i];
      if ( t.trigger ) delete t.trigger;
      if ( t.condition ) delete t.condition;
    }
    delete PRIVATE(this)->transitions;
    PRIVATE(this)->transitions = NULL;
  }

  if ( PRIVATE(this)->aborts ) {
    int i = 0;
    int max = PRIVATE(this)->aborts->getLength();
    for ( i = 0; i < max; i++ ) {
      Transition t = (*(PRIVATE(this)->aborts))[i];
      if ( t.trigger ) delete t.trigger;
      if ( t.condition ) delete t.condition;
    }
    delete PRIVATE(this)->aborts;
    PRIVATE(this)->aborts = NULL;
  }

  if ( PRIVATE(this)->finishes ) {
    int i = 0;
    int max = PRIVATE(this)->finishes->getLength();
    for ( i = 0; i < max; i++ ) {
      Transition t = (*(PRIVATE(this)->finishes))[i];
      if ( t.trigger ) delete t.trigger;
      if ( t.condition ) delete t.condition;
    }
    delete PRIVATE(this)->finishes;
    PRIVATE(this)->finishes = NULL;
  }

  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

SbName
NbNavigationMode::getModeName(void) const
{
  return PRIVATE(this)->modename;
}

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
*/

SoNode *
NbNavigationMode::getSceneGraph(void)
{
  return NULL;
}

void
NbNavigationMode::addTransition(NbNavigationMode * newmode,
				const SoEvent * event,
				SbBool stack,
				const SoEvent * condition)
{
  Transition t;
  t.trigger = SoEvent_Clone(event);
  if ( condition ) {
    t.condition = SoEvent_Clone(condition);
  } else {
    t.condition = NULL;
  }
  t.mode = newmode;
  if ( PRIVATE(this)->transitions == NULL ) {
    PRIVATE(this)->transitions = new SbList<Transition>;
  }
  PRIVATE(this)->transitions->append(t);
}

void
NbNavigationMode::addAbort(const SoEvent * event,
			   const SoEvent * condition)
{
  Transition t;
  t.trigger = SoEvent_Clone(event);
  if ( condition ) {
    t.condition = SoEvent_Clone(condition);
  } else {
    t.condition = NULL;
  }
  t.mode = NULL;
  if ( PRIVATE(this)->aborts == NULL ) {
    PRIVATE(this)->aborts = new SbList<Transition>;
  }
  PRIVATE(this)->aborts->append(t);
}

void
NbNavigationMode::addFinish(const SoEvent * event,
			    const SoEvent * condition)
{
  Transition t;
  t.trigger = SoEvent_Clone(event);
  if ( condition ) {
    t.condition = SoEvent_Clone(condition);
  } else {
    t.condition = NULL;
  }
  t.mode = NULL;
  if ( PRIVATE(this)->finishes == NULL ) {
    PRIVATE(this)->finishes = new SbList<Transition>;
  }
  PRIVATE(this)->finishes->append(t);
}

void
NbNavigationMode::activate(void)
{
  PRIVATE(this)->active = TRUE;
}

void
NbNavigationMode::deactivate(void)
{
  PRIVATE(this)->active = FALSE;
}

SbBool
NbNavigationMode::isActive(void) const
{
  if ( PRIVATE(this)->submode && PRIVATE(this)->submode->isActive() )
    return TRUE;
  return PRIVATE(this)->active;
}

// *************************************************************************

void
NbNavigationMode::init(const SoEvent * event, const NbNavigationInfo * info)
{
  // fprintf(stderr, "NbNavigationMode::init() %s\n",
  // 	  this->getModeName().getString());
  PRIVATE(this)->trigger = SoEvent_Clone(event);
  PRIVATE(this)->initpos = event->getPosition();
  PRIVATE(this)->prevpos = event->getPosition();
  PRIVATE(this)->currentpos = event->getPosition();
  PRIVATE(this)->active = TRUE;
}

void
NbNavigationMode::abort(const SoEvent * event, const NbNavigationInfo * info)
{
  // fprintf(stderr, "NbNavigationMode::abort() %s\n",
  // 	  this->getModeName().getString());
  info->restoreCamera();
  PRIVATE(this)->active = FALSE;
  PRIVATE(this)->trigger = NULL;
}

void
NbNavigationMode::finish(const SoEvent * event, const NbNavigationInfo * info)
{
  // fprintf(stderr, "NbNavigationMode::finish() %s\n",
  // 	  this->getModeName().getString());
  info->setCamera();
  PRIVATE(this)->active = FALSE;
  PRIVATE(this)->trigger = NULL;
}

// *************************************************************************

SbVec2s
NbNavigationMode::getInitialPosition(void) const
{
  return PRIVATE(this)->initpos;
}

SbVec2s
NbNavigationMode::getPreviousPosition(void) const
{
  return PRIVATE(this)->prevpos;
}

SbVec2s
NbNavigationMode::getCurrentPosition(void) const
{
  return PRIVATE(this)->currentpos;
}

SbVec2f
NbNavigationMode::getInitialNormalizedPosition(const NbNavigationInfo * info) const
{
  SbVec2s vp(info->getViewportSize());
  SbVec2s pos(this->getInitialPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

SbVec2f 
NbNavigationMode::getPreviousNormalizedPosition(const NbNavigationInfo * info) const
{
  SbVec2s vp(info->getViewportSize());
  SbVec2s pos(this->getPreviousPosition());
  return SbVec2f(float(pos[0])/float(vp[0]-1), float(pos[1]/float(vp[1]-1)));
}

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
  this->trigger = NULL;
  this->submode = NULL;
  this->active = FALSE;

  this->initpos.setValue(0, 0);
  this->prevpos.setValue(0, 0);
  this->currentpos.setValue(0, 0);

  this->transitions = NULL;
  this->finishes = NULL;
  this->aborts = NULL;
}
