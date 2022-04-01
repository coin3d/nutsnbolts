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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <cassert>
#include <cstdio>

#include <Inventor/SbDict.h>
#include <Inventor/SbPList.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodekits/SoBaseKit.h>

#include <NutsnBolts/misc/SoEvent.h>
#include <NutsnBolts/navigation/NbNavigationState.h>
#include <NutsnBolts/navigation/NbNavigationControl.h>
#include <NutsnBolts/navigation/NbNavigationMode.h>
#include <NutsnBolts/navigation/NbIdleMode.h>
#include <NutsnBolts/navigation/NbRotateMode.h>
#include <NutsnBolts/navigation/NbZoomMode.h>
#include <NutsnBolts/navigation/NbPanMode.h>
#include <NutsnBolts/navigation/NbPickMode.h>
#include <NutsnBolts/navigation/NbPitchMode.h>
#include <NutsnBolts/navigation/NbRollMode.h>
#include <NutsnBolts/navigation/NbYawMode.h>
#include <NutsnBolts/navigation/NbCenterMode.h>
#include <NutsnBolts/navigation/NbOrthoCenterMode.h>

#include <NutsnBolts/navigation/NbNavigationSystem.h>

// *************************************************************************

/*!
  \page navigation Navigation

  The SIM Nuts'n'Bolts NbSceneManager class implements a navigation
  system that allows for user interaction without having to deploy a
  full GUI toolkit viewer.

  This is done by attaching an object of type NbNavigationSystem to
  the NbSceneManager that will listen to all events sent to the scene
  and perform user-based camera navigation based on those events.

  A set of built-in navigation systems can be used for certain common
  navigation setups, but it is also possible to create ones own navigation
  modes, or patch together a new one from existing navigation submodes.

  Built-in modes are named in the following set of defines:
  \li NB_IDLER_SYSTEM
  \li NB_EXAMINER_SYSTEM
  \li NB_PANNER_SYSTEM
  \li NB_ZOOMER_SYSTEM
  \li NB_ROTATER_SYSTEM

  The NB_EXAMINER_SYSTEM is a navigation system that implements most of
  the examiner viewer navigation.  Not complete yet, but that is the
  goal.

  The NB_PANNER_SYSTEM just implements panning when the mouse button
  is pressed.  Similarly, the NB_ZOOMER_SYSTEM and NB_ROTATER_SYSTEM
  implement just zooming and rotating when the mouse is pressed.
*/

/*
  FIXME: there is never sent a mode change callback for the initial
  mode.  Maybe the first event to be sent to processEvent should
  invoke one?
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
  NbNavigationControl * ctrl;
  NbNavigationState * state;
  SbPList * modechangecbs;
  SbList<NbNavigationMode *> * modes;
  SbList<NbNavigationModeTransition *> * transitions;

  static SbDict * namedict;
};

SbDict * NbNavigationSystemP::namedict = NULL;

// *************************************************************************

/*!
  Initializes the built-in navigation systems.

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

  SoMouseButtonEvent * mwheeldown = new SoMouseButtonEvent;
  mwheeldown->setButton(SoMouseButtonEvent::BUTTON4);
  mwheeldown->setState(SoButtonEvent::DOWN);
  SoMouseButtonEvent * mwheelup = new SoMouseButtonEvent;
  mwheelup->setButton(SoMouseButtonEvent::BUTTON5);
  mwheelup->setState(SoButtonEvent::DOWN);
  
  // set up some standard navigation systems
  NbNavigationSystem * idler =
    new NbNavigationSystem(NB_IDLER_SYSTEM);
  NbIdleMode * idleridle =
    new NbIdleMode(NB_IDLER_IDLE_MODE);
  idler->addMode(idleridle);
  idler->addModeTransition(idleridle, INITIAL);

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
  examiner->addModeTransition(examineridle, examinerzoom,
			      STACK, mwheeldown);
  examiner->addModeTransition(examineridle, examinerzoom,
			      STACK, mwheelup);
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


  examiner->addModeTransition(examinerwaitforzoom, examinerzoom,
                              SWITCH, button1down);
  examiner->addModeTransition(examinerwaitforpan, examinerpan,
                              SWITCH, button1down);
  examiner->addModeTransition(examinerwaitforcenter, examinercenter,
                              SWITCH, button1down);

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

  NbNavigationSystem * picker = new NbNavigationSystem(NB_PICKER_SYSTEM);
  NbIdleMode * picker_idle = new NbIdleMode(NB_PICKER_IDLE_MODE);
  NbPickMode * picker_pick = new NbPickMode(NB_PICKER_PICK_MODE);
  picker->addMode(picker_idle);
  picker->addMode(picker_pick);
  picker->addModeTransition(picker_idle, INITIAL);
  picker->addModeTransition(picker_idle, picker_pick,
                            STACK, button1down);

  NbNavigationSystem::registerSystem(idler);
  NbNavigationSystem::registerSystem(examiner);
  NbNavigationSystem::registerSystem(rotater);
  NbNavigationSystem::registerSystem(panner);
  NbNavigationSystem::registerSystem(zoomer);
  NbNavigationSystem::registerSystem(centerer);
  NbNavigationSystem::registerSystem(picker);


  delete spacedown;
  delete escapedown;
  delete key_sdown;
  delete button1down;
  delete button1up;
  delete button2down;
  delete button2up;
  delete button3down;
  delete button3up;
  delete shiftdown;
  delete shiftup;
  delete ctrldown;
  delete ctrlup;
  delete mwheeldown;
  delete mwheelup;
}

/*!
  This function cleans up after the NbNavigationSystem class.

  \sa NbNavigationSystem::initClass
*/

void
NbNavigationSystem::cleanClass(void)
{
#define DELETE_SYSTEM(systemname) \
  do { \
    NbNavigationSystem * system = \
      NbNavigationSystem::getByName(systemname); \
    NbNavigationSystem::unregisterSystem(system); \
    delete system; \
  } while ( FALSE )

  DELETE_SYSTEM(NB_IDLER_SYSTEM);
  DELETE_SYSTEM(NB_EXAMINER_SYSTEM);
  DELETE_SYSTEM(NB_ZOOMER_SYSTEM);
  DELETE_SYSTEM(NB_ROTATER_SYSTEM);
  DELETE_SYSTEM(NB_PANNER_SYSTEM);
  DELETE_SYSTEM(NB_CENTERER_SYSTEM);
  DELETE_SYSTEM(NB_PICKER_SYSTEM);

#undef DELETE_SYSTEM

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
  if (NbNavigationSystem::getByName(name) != NULL) {
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
  if (NbNavigationSystem::getByName(name) == NULL) {
    return FALSE;
  }
  NbNavigationSystemP::namedict->remove((uintptr_t) name.getString());
  return TRUE;
}

/*!
  This function returns whether or not this particular navigation system
  instance is in the register.
*/

SbBool
NbNavigationSystem::isRegistered(NbNavigationSystem * system)
{
  assert(NbNavigationSystemP::namedict);
  SbName name = system->getName();
  NbNavigationSystem * registered = NbNavigationSystem::getByName(name);
  return (registered == system) ? TRUE : FALSE;
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

/*!
  This function returns a distinct copy of the navigation system defined
  by the given name, or NULL if no such navigation system has been 
*/

NbNavigationSystem *
NbNavigationSystem::createByName(SbName name)
{
  NbNavigationSystem * orig = NbNavigationSystem::getByName(name);
  if (!orig) return NULL;
  return orig->clone();
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
  This method creates a clone of this navigation system.  Only the mode
  network setup is copied - callback lists are not.
*/

NbNavigationSystem *
NbNavigationSystem::clone(void) const
{
  NbNavigationSystem * clone = new NbNavigationSystem(this->getName());
  int i;
  const int nummodes = PRIVATE(this)->modes->getLength();
  for ( i = 0; i < nummodes; i++ ) {
    PRIVATE(clone)->modes->append((*(PRIVATE(this)->modes))[i]->clone());
  }
  const int numtransitions = PRIVATE(this)->transitions->getLength();
  for ( i = 0; i < numtransitions; i++ ) {
    NbNavigationModeTransition * origt = (*(PRIVATE(this)->transitions))[i];
    NbNavigationModeTransition * tcopy = new NbNavigationModeTransition;
    tcopy->type = origt->type;
    tcopy->mode1 = NULL;
    if (origt->mode1) {
      int idx = PRIVATE(this)->modes->find(origt->mode1);
      assert(idx != -1);
      tcopy->mode1 = (*(PRIVATE(clone)->modes))[idx];
    }
    tcopy->mode2 = NULL;
    if (origt->mode2) {
      int idx = PRIVATE(this)->modes->find(origt->mode2);
      assert(idx != -1);
      tcopy->mode2 = (*(PRIVATE(clone)->modes))[idx];
    }
    tcopy->trigger = NULL;
    if (origt->trigger) {
      tcopy->trigger = SoEvent_Clone(origt->trigger);
    }
    tcopy->condition = NULL;
    if (origt->condition) {
      tcopy->condition = SoEvent_Clone(origt->condition);
    }
    PRIVATE(clone)->transitions->append(tcopy);
    if (tcopy->type == INITIAL) {
      PRIVATE(clone)->state->reset();
      PRIVATE(clone)->state->push(tcopy->mode1, NULL);
    }
  }
  return clone;
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
  PRIVATE(this)->ctrl->setSceneGraph(scenegraph);
}

/*!
  Sets the pointer to the camera that the navigation system should
  manipulate.
*/

void
NbNavigationSystem::setCamera(SoCamera * camera)
{
  PRIVATE(this)->ctrl->setCamera(camera);
}

/*!
  Sets the viewport region information needed for interpreting mouse events
  correctly.
*/

void
NbNavigationSystem::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->ctrl->setViewport(vp);
}

/*!
  Repositions the camera so that the complete model gets in the view.
*/

void
NbNavigationSystem::viewAll(void)
{
  PRIVATE(this)->ctrl->viewAll();
}

/*!
  This method places the camera so that it has the best view
  possible of the part \a path points to, given the in-vector and
  up-vector constraints.
*/

void
NbNavigationSystem::viewPart(SoPath * path, const SbVec3f & in, const SbVec3f & up)
{
  PRIVATE(this)->ctrl->viewPart(path, in, up);
}

/*!
  This method finds the first path to \a node and invokes the
  path-based viewPart() instead.  It's just a convenience function for
  avoiding to have to find the path yourself.

  \sa viewPart
*/

void
NbNavigationSystem::viewPart(SoNode * node, const SbVec3f & in, const SbVec3f & up)
{
  SoNode * root = PRIVATE(this)->ctrl->getSceneGraph();
  if (!root) return;
  SbBool searchingchildren = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  SoSearchAction sa;
  sa.setInterest(SoSearchAction::FIRST);
  sa.setNode(node);
  sa.apply(root);
  SoBaseKit::setSearchingChildren(searchingchildren);
  if (!sa.getPath()) return;
  this->viewPart(sa.getPath(), in, up);
}

/*!
  Returns the navigation control class used by the navigation
  systems.
*/

NbNavigationControl *
NbNavigationSystem::getNavigationControl(void) const
{
  return PRIVATE(this)->ctrl;
}

/*!
  Adds a callback that will be called each time the navigation
  system switches navigation mode.  Intended usage for this is for the
  application to switch mouse cursor representation and similar
  things.

  \sa removeModeChangeCallback, getCurrentModeName
*/

void
NbNavigationSystem::addModeChangeCallback(NbNavigationModeChangeCB * cb, void * closure)
{
  if (!PRIVATE(this)->modechangecbs) {
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
  if (!PRIVATE(this)->modechangecbs) return;
  const int max = PRIVATE(this)->modechangecbs->getLength();
  assert((max % 2) == 0);
  int i;
  for (i = 0; i < max; i += 2) {
    NbNavigationModeChangeCB * cb2 =
      (NbNavigationModeChangeCB *) (*(PRIVATE(this)->modechangecbs))[i];
    void * closure2 = (*(PRIVATE(this)->modechangecbs))[i+1];
    if (cb == cb2 && closure == closure2) {
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
  if (!PRIVATE(this)->modechangecbs) return;
  const int max = PRIVATE(this)->modechangecbs->getLength();
  assert((max % 2) == 0);
  int i;
  for (i = 0; i < max; i += 2) {
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
  assert(PRIVATE(this)->ctrl);
  NbNavigationMode * mode = PRIVATE(this)->state->getMode();
  if (mode == NULL) return FALSE;

  SbBool retval = FALSE;

  int i;
  const int max = PRIVATE(this)->transitions->getLength();
  // check if we're doing a mode transition
  for (i = 0; i < max; i++) {
    NbNavigationModeTransition * transition =
      (*(PRIVATE(this)->transitions))[i];
    if (transition->mode1 != mode) continue;
    if (transition->type == INITIAL) continue; // trigger is NULL
    if (!SoEvent_Equals(transition->trigger, event)) continue;
    if (transition->condition != NULL) {
      const SoEvent * curtrigger = PRIVATE(this)->state->getTrigger();
      if (curtrigger == NULL) {
        continue;
      }
      if (!SoEvent_Equals(transition->condition, curtrigger)) {
        // SoEvent_Dump(stderr, curtrigger);
        // SoEvent_Dump(stderr, transition->condition);
        continue;
      }
    }
    // we are doing a mode transition
    switch (transition->type) {
    case INITIAL:
      assert(0 && "crazy!");
      break;

    case FINISH:
    case ABORT:
      mode->processEvent(event, PRIVATE(this)->ctrl);
      if (transition->type == FINISH) {
        mode->finish(event, PRIVATE(this)->ctrl);
      } else {
        mode->abort(event, PRIVATE(this)->ctrl);
      }
      PRIVATE(this)->state->pop();
      mode = PRIVATE(this)->state->getMode();
      if (mode) {
        this->invokeModeChangeCallbacks();
        mode->init(event, PRIVATE(this)->ctrl);
        mode->processEvent(event, PRIVATE(this)->ctrl);
      }
      retval = TRUE; // transitions should always be considered handled events
      break;
      
    case STACK:
    case SWITCH:
      mode->processEvent(event, PRIVATE(this)->ctrl);
      mode->finish(event, PRIVATE(this)->ctrl);
      if (transition->type == SWITCH)
        PRIVATE(this)->state->pop();
      PRIVATE(this)->state->push(transition->mode2, event);
      mode = PRIVATE(this)->state->getMode();
      this->invokeModeChangeCallbacks();
      mode->init(event, PRIVATE(this)->ctrl);
      mode->processEvent(event, PRIVATE(this)->ctrl);
      retval = TRUE; // transitions should always be considered handled events
      break;
    }
    break;
  }

  // no transition - just regular event processing
  if ( mode->processEvent(event, PRIVATE(this)->ctrl) ) {
    retval = TRUE;
  }

  // check if mode aborted/closed itself
  while (mode->isAborted() || mode->isFinished()) {
    if (mode->isAborted()) {
      mode->abort(event, PRIVATE(this)->ctrl);
    } else {
      mode->finish(event, PRIVATE(this)->ctrl);
    }
    PRIVATE(this)->state->pop();
    this->invokeModeChangeCallbacks();
    mode = PRIVATE(this)->state->getMode();
    mode->init(event, PRIVATE(this)->ctrl);
    mode->processEvent(event, PRIVATE(this)->ctrl);
    retval = TRUE; // transition means handled...
  }

  return retval;
}

/*!
  Adds a submode to a navigation system.
*/

void
NbNavigationSystem::addMode(NbNavigationMode * mode)
{
  assert(PRIVATE(this)->modes);
  if (PRIVATE(this)->modes->find(mode) != -1) return;
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

  if (type == INITIAL) {
    PRIVATE(this)->state->reset();
    PRIVATE(this)->state->push(mode, NULL);
  }

  NbNavigationModeTransition * transition = new NbNavigationModeTransition;
  transition->type = type;
  transition->mode1 = mode;
  transition->mode2 = NULL;
  transition->trigger = NULL;
  if (trigger) {
    transition->trigger = SoEvent_Clone(trigger);
  }
  transition->condition = NULL;
  if (condition) {
    transition->condition = SoEvent_Clone(condition);
  }
  PRIVATE(this)->transitions->append(transition);
}

/*!
  Adds a mode transition.
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
  assert(trigger);

  NbNavigationModeTransition * transition = new NbNavigationModeTransition;
  transition->type = type;
  transition->mode1 = mode1;
  transition->mode2 = mode2;
  transition->trigger = SoEvent_Clone(trigger);
  transition->condition = NULL;
  if (condition) {
    transition->condition = SoEvent_Clone(condition);
  }
  PRIVATE(this)->transitions->append(transition);
}

/*!
  Returns the name of the current submode.
*/

SbName
NbNavigationSystem::getCurrentModeName(void) const
{
  NbNavigationMode * mode = PRIVATE(this)->state->getMode();
  if (!mode) return SbName("");
  return mode->getModeName();
}

/*!
  Returns the submode with the given name.

  NULL is returned if no mode with the given name exists in the system.
*/

NbNavigationMode *
NbNavigationSystem::getMode(SbName name) const
{
  const int num = PRIVATE(this)->modes->getLength();
  int i;
  for ( i = 0; i < num; i++ ) {
    NbNavigationMode * mode = (*(PRIVATE(this)->modes))[i];
    if (mode->getModeName() == name) {
      return mode;
    }
  }
  return NULL;
}

/*!
  Returns the object for the current submode.
*/

NbNavigationMode *
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
  this->ctrl = new NbNavigationControl;
  this->state = new NbNavigationState;
  this->modechangecbs = NULL;
  this->modes = new SbList<NbNavigationMode *>;
  this->transitions = new SbList<NbNavigationModeTransition *>;
}

NbNavigationSystemP::~NbNavigationSystemP(void)
{
  if (this->modechangecbs) {
    delete this->modechangecbs;
  }
  do { // clean up transitions
    const int num = this->transitions->getLength();
    int i;
    for (i = 0; i < num; i++) {
      NbNavigationModeTransition * transition =	(*(this->transitions))[i];
      if (transition->trigger) { delete transition->trigger; }
      if (transition->condition) { delete transition->condition; }
      delete transition;
    }
    delete this->transitions;
  } while (FALSE);
  do {
    const int num = this->modes->getLength();
    int i;
    for (i = 0; i < num; i++) {
      NbNavigationMode * mode = (*(this->modes))[i];
      delete mode;
    }
    delete this->modes;
  } while (FALSE);
  delete this->ctrl;
  delete this->state;
}

// *************************************************************************
