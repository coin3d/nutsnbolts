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
#include <stdio.h>

#include <Inventor/SbDict.h>
#include <Inventor/SbPList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>

#include <NutsnBolts/misc/SoEvent.h>
#include <NutsnBolts/navigation/NbNavigationState.h>
#include <NutsnBolts/navigation/NbNavigationInfo.h>
#include <NutsnBolts/navigation/NbNavigationMode.h>
#include <NutsnBolts/navigation/NbIdleMode.h>
#include <NutsnBolts/navigation/NbRotateMode.h>
#include <NutsnBolts/navigation/NbZoomMode.h>
#include <NutsnBolts/navigation/NbPanMode.h>
#include <NutsnBolts/navigation/NbCenterMode.h>

#include <NutsnBolts/navigation/NbNavigationSystem.h>

// *************************************************************************

/*!
  \page navigation Navigation

  The SIM Nuts'n'Bolts NbSceneManager class implements a navigation
  system that allows for user interaction without having to deploy a
  full GUI toolkit viewer.

  This is done by atatching an object of type NbNavigationSystem to
  the NbSceneManager that will listen to all events sent to the scene
  and perform user-based camera navigation based on those events.

  A set of builtin navigation systems can be used for certain common
  navigation setups, but it is also possible to create ones own navigation
  modes, or  patch together a new one from existing navigation submodes.

  Builtin modes are named in the following set of defines:
  \li NB_EXAMINER_SYSTEM
  \li NB_PANNER_SYSTEM
  \li NB_ZOOMER_SYSTEM
  \li NB_ROTATER_SYSTEM

  The NB_EXAMINER_SYSTEM is a navigation system that implements most of
  the examiner viewer navigation.  Not complete yet, but that is the
  goal.

  The NB_PANNER_SYSTEM just implements panning when the mouse button
  is pressed.  Similarly, the NB_ZOOMER_SYSTEM and NB_ROTATER_SYSTEM
  implements just zooming and rotating when the mouse is pressed.
*/


/*!
  \class NbNavigationSystem NutsnBolts/navigation/NbNavigationSystem.h
  \brief Class for managing user-based viewer navigation.

  Container class for navigation systems.

  \ingroup navigation
*/

/*!
  \enum NbNavigationSystem::TransitionType

  This enum defines the types of mode transitions supported by the
  NbNavigationSystem class.
*/

/*!
  \enum NbNavigationSystem::INITIAL
  
  This transition is used to set the default/initial submode of the
  navigation system.
*/

/*!
  \enum NbNavigationSystem::STACK

  This mode transition means that the target mode of the transition is
  stacked on top of the current mode, and this mode will be returned
  to when the target mode is done.
*/

/*!
  \enum NbNavigationSystem::SWITCH

  This mode transition means that the target mode will replace the
  current mode in the mode stack so the current mode is not the mode
  returned to when the new mode finishes.
*/

/*!
  \enum NbNavigationSystem::FINISH

  This mode transition will make the system return to the previous
  mode - the mode under it on the mode stack.
*/

/*!
  \enum NbNavigationSystem::ABORT

  This mode transition will abort the current mode and make the system
  return to the previous mode - the mode under it on the mode stack.
*/

/*!
  \typedef void NbNavigationModeChangeCB(void * closure, NbNavigationSystem * system)

  This function type defines the signature the NbNavigationSystem mode
  change callbacks must use.

  \relates NbNavigationSystem

  \sa NbNavigationSystem::addModeChangeCallback, NbNavigationSystem::removeModeChangeCallback
*/

// *************************************************************************
// private type declarations

struct NbNavigationModeTransition {
  NbNavigationSystem::TransitionType type;
  NbNavigationMode * mode1;
  NbNavigationMode * mode2;
  const SoEvent * trigger;
  const SoEvent * condition;
};

class NbNavigationSystemP {
public:
  NbNavigationSystemP(NbNavigationSystem * api);
  ~NbNavigationSystemP(void);

  SbName name;
  NbNavigationInfo * info;
  NbNavigationState * state;
  SbPList * modechangecbs;
  SbList<NbNavigationMode *> * modes;
  SbList<NbNavigationModeTransition *> * transitions;

  static SbDict * namedict;
};

SbDict * NbNavigationSystemP::namedict = NULL;

// *************************************************************************

/*!
  Initializes the builtin navigation systems.

  \sa NbNavigationSystem::cleanClass
*/

void
NbNavigationSystem::initClass(void)
{
  NbNavigationSystemP::namedict = new SbDict;

  // create events to listen for
  SoKeyboardEvent * escapedown = new SoKeyboardEvent;
  escapedown->setKey(SoKeyboardEvent::ESCAPE);
  escapedown->setState(SoButtonEvent::DOWN);

  SoKeyboardEvent * spacedown = new SoKeyboardEvent;
  spacedown->setKey(SoKeyboardEvent::SPACE);
  spacedown->setState(SoButtonEvent::DOWN);

  SoKeyboardEvent * shiftdown = new SoKeyboardEvent;
  shiftdown->setKey(SoKeyboardEvent::LEFT_SHIFT);
  shiftdown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * shiftup = new SoKeyboardEvent;
  shiftup->setKey(SoKeyboardEvent::LEFT_SHIFT);
  shiftup->setState(SoButtonEvent::UP);

  SoKeyboardEvent * ctrldown = new SoKeyboardEvent;
  ctrldown->setKey(SoKeyboardEvent::LEFT_CONTROL);
  ctrldown->setState(SoButtonEvent::DOWN);
  SoKeyboardEvent * ctrlup = new SoKeyboardEvent;
  ctrlup->setKey(SoKeyboardEvent::LEFT_CONTROL);
  ctrlup->setState(SoButtonEvent::UP);

  SoKeyboardEvent * key_sdown = new SoKeyboardEvent;
  key_sdown->setKey(SoKeyboardEvent::S);
  key_sdown->setState(SoButtonEvent::DOWN);

  SoMouseButtonEvent * button1down = new SoMouseButtonEvent;
  button1down->setButton(SoMouseButtonEvent::BUTTON1);
  button1down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button1up = new SoMouseButtonEvent;
  button1up->setButton(SoMouseButtonEvent::BUTTON1);
  button1up->setState(SoButtonEvent::UP);
  
  SoMouseButtonEvent * button2down = new SoMouseButtonEvent;
  button2down->setButton(SoMouseButtonEvent::BUTTON2);
  button2down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button2up = new SoMouseButtonEvent;
  button2up->setButton(SoMouseButtonEvent::BUTTON2);
  button2up->setState(SoButtonEvent::UP);
  
  SoMouseButtonEvent * button3down = new SoMouseButtonEvent;
  button3down->setButton(SoMouseButtonEvent::BUTTON3);
  button3down->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * button3up = new SoMouseButtonEvent;
  button3up->setButton(SoMouseButtonEvent::BUTTON3);
  button3up->setState(SoButtonEvent::UP);
  
  // set up some standard navigation systems

  NbNavigationSystem * examiner =
    new NbNavigationSystem(NB_EXAMINER_SYSTEM);

  NbIdleMode * examineridle =
    new NbIdleMode(NB_EXAMINER_IDLE_MODE);
  NbRotateMode * examinerrotate =
    new NbRotateMode(NB_EXAMINER_ROTATE_MODE);
  NbIdleMode * examinerwaitforzoom =
    new NbIdleMode(NB_EXAMINER_WAITFORZOOM_MODE);
  NbZoomMode * examinerzoom =
    new NbZoomMode(NB_EXAMINER_ZOOM_MODE);
  NbIdleMode * examinerwaitforpan =
    new NbIdleMode(NB_EXAMINER_WAITFORPAN_MODE);
  NbPanMode * examinerpan =
    new NbPanMode(NB_EXAMINER_PAN_MODE);
  NbIdleMode * examinerwaitforcenter =
    new NbIdleMode(NB_EXAMINER_WAITFORCENTER_MODE);
  NbCenterMode * examinercenter =
    new NbCenterMode(NB_EXAMINER_CENTER_MODE);

  examiner->addMode(examineridle);
  examiner->addMode(examinerrotate);
  examiner->addMode(examinerwaitforzoom);
  examiner->addMode(examinerwaitforpan);
  examiner->addMode(examinerwaitforcenter);
  examiner->addMode(examinerzoom);
  examiner->addMode(examinerpan);
  examiner->addMode(examinercenter);

  examiner->addModeTransition(examineridle, INITIAL);
  examiner->addModeTransition(examineridle, examinerrotate,
			      STACK, button1down);
  examiner->addModeTransition(examineridle, examinerwaitforzoom,
			      STACK, shiftdown);
  examiner->addModeTransition(examineridle, examinerwaitforpan,
			      STACK, ctrldown);
  examiner->addModeTransition(examineridle, examinerzoom,
			      STACK, button2down);
  examiner->addModeTransition(examineridle, examinerpan,
			      STACK, button3down);
  examiner->addModeTransition(examineridle, examinerwaitforcenter,
			      STACK, key_sdown);

  examiner->addModeTransition(examinerrotate, FINISH, button1up, button1down);
  examiner->addModeTransition(examinerrotate, ABORT, escapedown);

  examiner->addModeTransition(examinerzoom, FINISH, button1up, button1down);
  examiner->addModeTransition(examinerzoom, FINISH, button2up, button2down);
  examiner->addModeTransition(examinerzoom, ABORT, escapedown);

  examiner->addModeTransition(examinerpan, FINISH, button1up, button1down);
  examiner->addModeTransition(examinerpan, FINISH, button3up, button3down);
  examiner->addModeTransition(examinerpan, ABORT, escapedown);

  examiner->addModeTransition(examinerwaitforzoom, FINISH, shiftup, shiftdown);
  examiner->addModeTransition(examinerwaitforpan, FINISH, ctrlup, ctrldown);
  examiner->addModeTransition(examinerwaitforcenter, FINISH, key_sdown);


  examiner->addModeTransition(examinerwaitforzoom, examinerzoom, SWITCH, button1down);
  examiner->addModeTransition(examinerwaitforpan, examinerpan, SWITCH, button1down);
  examiner->addModeTransition(examinerwaitforcenter, examinercenter,
			      SWITCH, button1down);

  examiner->addModeTransition(examinercenter, FINISH, button1up);


  NbNavigationSystem * rotater = new NbNavigationSystem(NB_ROTATER_SYSTEM);
  NbIdleMode * rotater_idle = new NbIdleMode(NB_ROTATER_IDLE_MODE);
  NbRotateMode * rotater_rotate = new NbRotateMode(NB_ROTATER_ROTATE_MODE);
  rotater->addMode(rotater_idle);
  rotater->addMode(rotater_rotate);
  rotater->addModeTransition(rotater_idle, INITIAL);
  rotater->addModeTransition(rotater_idle, rotater_rotate,
			     STACK, button1down);
  rotater->addModeTransition(rotater_rotate, rotater_rotate,
			     SWITCH, spacedown);
  rotater->addModeTransition(rotater_rotate, FINISH, button1up);
  rotater->addModeTransition(rotater_rotate, ABORT, escapedown);


  NbNavigationSystem * panner = new NbNavigationSystem(NB_PANNER_SYSTEM);
  NbIdleMode * panner_idle = new NbIdleMode(NB_PANNER_IDLE_MODE);
  NbPanMode * panner_pan = new NbPanMode(NB_PANNER_PAN_MODE);
  panner->addMode(panner_idle);
  panner->addMode(panner_pan);
  panner->addModeTransition(panner_idle, INITIAL);
  panner->addModeTransition(panner_idle, panner_pan,
			    STACK, button1down);
  panner->addModeTransition(panner_pan, panner_pan,
			    SWITCH, spacedown);
  panner->addModeTransition(panner_pan, FINISH, button1up);
  panner->addModeTransition(panner_pan, ABORT, escapedown);


  NbNavigationSystem * zoomer = new NbNavigationSystem(NB_ZOOMER_SYSTEM);
  NbIdleMode * zoomer_idle = new NbIdleMode(NB_ZOOMER_IDLE_MODE);
  NbZoomMode * zoomer_zoom = new NbZoomMode(NB_ZOOMER_ZOOM_MODE);
  zoomer->addMode(zoomer_idle);
  zoomer->addMode(zoomer_zoom);
  zoomer->addModeTransition(zoomer_idle, INITIAL);
  zoomer->addModeTransition(zoomer_idle, zoomer_zoom,
			    STACK, button1down);
  zoomer->addModeTransition(zoomer_zoom, zoomer_zoom,
			    SWITCH, spacedown);
  zoomer->addModeTransition(zoomer_zoom, FINISH, button1up);
  zoomer->addModeTransition(zoomer_zoom, ABORT, escapedown);

  NbNavigationSystem * centerer = new NbNavigationSystem(NB_CENTERER_SYSTEM);
  NbIdleMode * centerer_idle = new NbIdleMode(NB_CENTERER_IDLE_MODE);
  NbCenterMode * centerer_center = new NbCenterMode(NB_CENTERER_CENTER_MODE);

  centerer->addMode(centerer_idle);
  centerer->addMode(centerer_center);
  centerer->addModeTransition(centerer_idle, INITIAL);
  centerer->addModeTransition(centerer_idle, centerer_center,
			      STACK, button1down);
  centerer->addModeTransition(centerer_center, FINISH,
			      button1up, button1down);

  NbNavigationSystem::registerSystem(examiner);
  NbNavigationSystem::registerSystem(rotater);
  NbNavigationSystem::registerSystem(panner);
  NbNavigationSystem::registerSystem(zoomer);
  NbNavigationSystem::registerSystem(centerer);

  // FIXME: delete events
}

/*!
  Thisfunction cleans up after the NbNavigationSystem class.

  \sa NbNavigationSystem::initClass
*/

void
NbNavigationSystem::cleanClass(void)
{
  NbNavigationSystem * examiner =
    NbNavigationSystem::getByName(NB_EXAMINER_SYSTEM);
  NbNavigationSystem::unregisterSystem(examiner);
  delete examiner;

  delete NbNavigationSystemP::namedict;
  NbNavigationSystemP::namedict = NULL;
}

/*!
  This function registers a navigation system object so it can later
  be fetched by name through NbNavigationSystem::getByName().

  Registered navigation systems can be enabled for an NbSceneManager
  through use of an NbViewerNavigationMode node in the scene graph.

  \sa NbNavigationSystem::unregisterSystem, NbNavigationSystem::getByName
*/

SbBool
NbNavigationSystem::registerSystem(NbNavigationSystem * system)
{
  assert(NbNavigationSystemP::namedict);
  SbName name = system->getName();
  if ( NbNavigationSystem::getByName(name) != NULL ) {
    return FALSE;
  }
  NbNavigationSystemP::namedict->enter((uintptr_t) name.getString(), system);
  return TRUE;
}

/*!
  This function unregisters a navigation system from the dictionary.

  \sa NbNavigationSystem::registerSystem, NbNavigationSystem::getByName
*/

SbBool
NbNavigationSystem::unregisterSystem(NbNavigationSystem * system)
{
  assert(NbNavigationSystemP::namedict);
  SbName name = system->getName();
  if ( NbNavigationSystem::getByName(name) == NULL ) {
    return FALSE;
  }
  NbNavigationSystemP::namedict->remove((uintptr_t) name.getString());
  return TRUE;
}

/*!
  This function returns the navigation system that has been registered
  under the given name.  NULL is returned if no such system has been
  registered.

  \sa NbNavigationSystem::registerSystem, NbNavigationSystem::unregisterSystem
*/

NbNavigationSystem *
NbNavigationSystem::getByName(SbName name)
{
  assert(NbNavigationSystemP::namedict);
  void * ptr = NULL;
  NbNavigationSystemP::namedict->find((uintptr_t) name.getString(), ptr);
  return (NbNavigationSystem *) ptr;
}

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.  The \a name argument is the name of the system, the
  name it will be registered under with
  NbSceneManager::registerSystem().
*/

NbNavigationSystem::NbNavigationSystem(SbName name)
{
  PRIVATE(this) = new NbNavigationSystemP(this);
  PRIVATE(this)->name = SbName(name);
}

/*!
  Destructor.
*/

NbNavigationSystem::~NbNavigationSystem(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Returns the name of the navigation system.
*/

SbName
NbNavigationSystem::getName(void) const
{
  return PRIVATE(this)->name;
}

void
NbNavigationSystem::setSceneGraph(SoNode * scenegraph)
{
  PRIVATE(this)->info->setSceneGraph(scenegraph);
}

/*!
  Sets the pointer to the camera that the navigation system should
  manipulate.
*/

void
NbNavigationSystem::setCamera(SoCamera * camera)
{
  PRIVATE(this)->info->setCamera(camera);
}

/*!
  Sets the viewport region information needed for interpreting mouse events
  correctly.
*/

void
NbNavigationSystem::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->info->setViewport(vp);
}

/*!
  Adds a callback that will be called each time the navigation
  system switches navigation mode.

  \sa removeModeChangeCallback, getCurrentModeName
*/

void
NbNavigationSystem::addModeChangeCallback(NbNavigationModeChangeCB * cb, void * closure)
{
  if ( !PRIVATE(this)->modechangecbs ) {
    PRIVATE(this)->modechangecbs = new SbPList;
  }
  PRIVATE(this)->modechangecbs->append((void *) cb);
  PRIVATE(this)->modechangecbs->append(closure);
}

/*!
  Removes a callback set with addModeChangeCallback().

  \sa addModeChangeCallback
*/

void
NbNavigationSystem::removeModeChangeCallback(NbNavigationModeChangeCB * cb, void * closure)
{
  if ( !PRIVATE(this)->modechangecbs ) return;
  const int max = PRIVATE(this)->modechangecbs->getLength();
  assert((max % 2) == 0);
  int i;
  for ( i = 0; i < max; i += 2 ) {
    NbNavigationModeChangeCB * cb2 =
      (NbNavigationModeChangeCB *) (*(PRIVATE(this)->modechangecbs))[i];
    void * closure2 = (*(PRIVATE(this)->modechangecbs))[i+1];
    if ( cb == cb2 && closure == closure2 ) {
      PRIVATE(this)->modechangecbs->remove(i+1);
      PRIVATE(this)->modechangecbs->remove(i);
      return;
    }
  }
}

/*!
  Invokes all the mode change callbacks.

  \sa addModeChangeCallback, removeModeChangeCallback
*/

void
NbNavigationSystem::invokeModeChangeCallbacks(void)
{
  if ( !PRIVATE(this)->modechangecbs ) return;
  const int max = PRIVATE(this)->modechangecbs->getLength();
  assert((max % 2) == 0);
  int i;
  for ( i = 0; i < max; i += 2 ) {
    NbNavigationModeChangeCB * cb =
      (NbNavigationModeChangeCB *) (*(PRIVATE(this)->modechangecbs))[i];
    void * closure = (*(PRIVATE(this)->modechangecbs))[i+1];
    cb(closure, this);
  }
}

/*!
  Processes an event.  Returns TRUE if a mode used the event, and FALSE
  if the event didn't result in anything.
*/

SbBool
NbNavigationSystem::processEvent(const SoEvent * event)
{
  assert(event);
  assert(PRIVATE(this)->state);
  assert(PRIVATE(this)->info);
  NbNavigationMode * mode = PRIVATE(this)->state->getMode();
  if ( mode == NULL ) return FALSE;
  // fprintf(stderr, "NbNavigationSystem::processEvent()\n");

  const int max = PRIVATE(this)->transitions->getLength();
  // fprintf(stderr, "system has %d transitions\n", max);

  // check if we're doing a mode transition
  int i;
  for ( i = 0; i < max; i++ ) {
    NbNavigationModeTransition * transition =
      (*(PRIVATE(this)->transitions))[i];
    if ( transition->mode1 != mode ) continue;
    // fprintf(stderr, "trying transition %d\n", i);
    if ( transition->type == INITIAL ) continue; // trigger is NULL
    if ( ! SoEvent_Equals(transition->trigger, event) ) continue;

    // we are doing a mode transition
    // fprintf(stderr, "triggering transition %d\n", i);
    switch ( transition->type ) {
    case INITIAL:
      assert(0 && "crazy!");
      break;
    case FINISH:
    case ABORT:
      mode->processEvent(event, PRIVATE(this)->info);
      if ( transition->type == FINISH ) {
	mode->finish(event, PRIVATE(this)->info);
      } else {
	mode->abort(event, PRIVATE(this)->info);
      }
      PRIVATE(this)->state->pop();
      mode = PRIVATE(this)->state->getMode();
      if ( mode ) {
	this->invokeModeChangeCallbacks();
	mode->init(event, PRIVATE(this)->info);
	mode->processEvent(event, PRIVATE(this)->info);
      }
      return TRUE;

    case STACK:
    case SWITCH:
      mode->processEvent(event, PRIVATE(this)->info);
      mode->finish(event, PRIVATE(this)->info);
      if ( transition->type == SWITCH )
	PRIVATE(this)->state->pop();
      PRIVATE(this)->state->push(transition->mode2, event);
      mode = PRIVATE(this)->state->getMode();
      this->invokeModeChangeCallbacks();
      mode->init(event, PRIVATE(this)->info);
      mode->processEvent(event, PRIVATE(this)->info);
      return TRUE;
    }
  }

  // no transition - just regular event processing
  return mode->processEvent(event, PRIVATE(this)->info);
}

/*!
  Adds a submode to a navigation system.
*/

void
NbNavigationSystem::addMode(NbNavigationMode * mode)
{
  assert(PRIVATE(this)->modes);
  if ( PRIVATE(this)->modes->find(mode) != -1 ) return;
  PRIVATE(this)->modes->append(mode);
}

/*!
  Adds a mode transition.
*/

void
NbNavigationSystem::addModeTransition(NbNavigationMode * mode,
				      TransitionType type,
				      const SoEvent * trigger,
				      const SoEvent * condition)
{
  assert(PRIVATE(this)->modes);
  assert(PRIVATE(this)->modes->find(mode) != -1);
  assert(mode);
  assert(type == INITIAL || type == ABORT || type == FINISH);
  assert(trigger || type == INITIAL);

  if ( type == INITIAL ) {
    PRIVATE(this)->state->reset();
    PRIVATE(this)->state->push(mode, NULL);
  }

  NbNavigationModeTransition * transition = new NbNavigationModeTransition;
  transition->type = type;
  transition->mode1 = mode;
  transition->mode2 = NULL;
  transition->trigger = trigger;
  transition->condition = condition;

  PRIVATE(this)->transitions->append(transition);
}

/*!
  Ads a mode transition.
*/

void
NbNavigationSystem::addModeTransition(NbNavigationMode * mode1,
				      NbNavigationMode * mode2,
				      TransitionType type,
				      const SoEvent * trigger,
				      const SoEvent * condition)
{
  assert(PRIVATE(this)->modes);
  assert(mode1); assert(mode2);
  assert(PRIVATE(this)->modes->find(mode1) != -1);
  assert(PRIVATE(this)->modes->find(mode2) != -1);
  assert(type == STACK || type == SWITCH);

  NbNavigationModeTransition * transition = new NbNavigationModeTransition;
  transition->type = type;
  transition->mode1 = mode1;
  transition->mode2 = mode2;
  transition->trigger = trigger;
  transition->condition = condition;

  PRIVATE(this)->transitions->append(transition);
}

/*!
  Returns the name of the current submode.
*/

SbName
NbNavigationSystem::getCurrentModeName(void) const
{
  NbNavigationMode * mode = PRIVATE(this)->state->getMode();
  if ( !mode ) return SbName("");
  return mode->getModeName();
}

/*!
  Returns the object for the current submode.
*/

const NbNavigationMode *
NbNavigationSystem::getCurrentMode(void) const
{
  return PRIVATE(this)->state->getMode();
}
 
// *************************************************************************

#undef PRIVATE

// *************************************************************************
// private implementation

NbNavigationSystemP::NbNavigationSystemP(NbNavigationSystem * api)
{
  this->info = new NbNavigationInfo;
  this->state = new NbNavigationState;
  this->modechangecbs = NULL;
  this->modes = new SbList<NbNavigationMode *>;
  this->transitions = new SbList<NbNavigationModeTransition *>;
}

NbNavigationSystemP::~NbNavigationSystemP(void)
{
  if ( this->modechangecbs ) {
    delete this->modechangecbs;
  }
  do { // clean up transitions
    const int num = this->transitions->getLength();
    int i;
    for ( i = 0; i < num; i++ ) {
      NbNavigationModeTransition * transition =
	(*(this->transitions))[i];
      delete transition;
    }
    delete this->transitions;
  } while ( FALSE );
  delete this->modes;
  delete this->info;
  delete this->state;
}

// *************************************************************************
