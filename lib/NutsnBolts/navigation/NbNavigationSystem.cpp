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

#include <NutsnBolts/navigation/NbNavigationSystem.h>

// *************************************************************************

/*!
  \class NbNavigationSystem NutsnBolts/navigation/NbNavigationSystem.h
  \brief Class for managing user-based viewer navigation.

  \ingroup navigation
*/

/*!
  \page navigation Navigation

  The Nuts'n'Bolts NbSceneManager class implements a navigation system
  that allows for user interaction without having to deploy a full GUI
  toolkit viewer.

*/
// \ingroup navigation

// examinermode
// idle
// examinermode:wait_to_seek
// examinermode:wait_to_pan
// examinermode:wait_to_zoom
// examiner:wait_to_dolly
// examiner:rotate
// examiner:spin
// examiner:pan
// examiner:zoom
// examiner:dolly
// examiner:seek

// *************************************************************************

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
    new NbIdleMode(examiner, NB_EXAMINER_IDLE_MODE);
  NbRotateMode * examinerrotate =
    new NbRotateMode(examiner, NB_EXAMINER_ROTATE_MODE);
  NbIdleMode * examinerwaitforzoom =
    new NbIdleMode(examiner, NB_EXAMINER_WAITFORZOOM_MODE);
  NbZoomMode * examinerzoom =
    new NbZoomMode(examiner, NB_EXAMINER_ZOOM_MODE);
  NbIdleMode * examinerwaitforpan =
    new NbIdleMode(examiner, NB_EXAMINER_WAITFORPAN_MODE);
  NbPanMode * examinerpan =
    new NbPanMode(examiner, NB_EXAMINER_PAN_MODE);

  examiner->addMode(examineridle);
  examiner->addMode(examinerrotate);
  examiner->addMode(examinerwaitforzoom);
  examiner->addMode(examinerwaitforpan);
  examiner->addMode(examinerzoom);
  examiner->addMode(examinerpan);

  examiner->addModeTransition(examineridle, INITIAL);
  examiner->addModeTransition(examineridle, examinerrotate, STACK, button1down);
  examiner->addModeTransition(examineridle, examinerwaitforzoom, STACK, shiftdown);
  examiner->addModeTransition(examineridle, examinerwaitforpan, STACK, ctrldown);


  examiner->addModeTransition(examinerrotate, FINISH, button1up);
  examiner->addModeTransition(examinerrotate, ABORT, escapedown);

  examiner->addModeTransition(examinerzoom, FINISH, button1up);
  examiner->addModeTransition(examinerzoom, ABORT, escapedown);


  examiner->addModeTransition(examinerwaitforzoom, examinerzoom, SWITCH, button1down);


  NbNavigationSystem * rotater = new NbNavigationSystem(NB_ROTATER_SYSTEM);
  NbIdleMode * rotater_idle = new NbIdleMode(rotater, NB_ROTATER_IDLE_MODE);
  NbRotateMode * rotater_rotate = new NbRotateMode(rotater, NB_ROTATER_ROTATE_MODE);
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
  NbIdleMode * panner_idle = new NbIdleMode(panner, NB_PANNER_IDLE_MODE);
  NbPanMode * panner_pan = new NbPanMode(panner, NB_PANNER_PAN_MODE);
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
  NbIdleMode * zoomer_idle = new NbIdleMode(zoomer, NB_ZOOMER_IDLE_MODE);
  NbZoomMode * zoomer_zoom = new NbZoomMode(zoomer, NB_ZOOMER_ZOOM_MODE);
  zoomer->addMode(zoomer_idle);
  zoomer->addMode(zoomer_zoom);
  zoomer->addModeTransition(zoomer_idle, INITIAL);
  zoomer->addModeTransition(zoomer_idle, zoomer_zoom,
			    STACK, button1down);
  zoomer->addModeTransition(zoomer_zoom, zoomer_zoom,
			    SWITCH, spacedown);
  zoomer->addModeTransition(zoomer_zoom, FINISH, button1up);
  zoomer->addModeTransition(zoomer_zoom, ABORT, escapedown);


  NbNavigationSystem::registerSystem(examiner);
  NbNavigationSystem::registerSystem(rotater);
  NbNavigationSystem::registerSystem(panner);
  NbNavigationSystem::registerSystem(zoomer);
}

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

NbNavigationSystem *
NbNavigationSystem::getByName(SbName name)
{
  assert(NbNavigationSystemP::namedict);
  void * ptr = NULL;
  NbNavigationSystemP::namedict->find((uintptr_t) name.getString(), ptr);
  return (NbNavigationSystem *) ptr;
}

// *************************************************************************

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

#define PRIVATE(obj) ((obj)->pimpl)

NbNavigationSystem::NbNavigationSystem(SbName name)
{
  PRIVATE(this) = new NbNavigationSystemP(this);
  PRIVATE(this)->name = SbName(name);
}

NbNavigationSystem::~NbNavigationSystem(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

SbName
NbNavigationSystem::getName(void) const
{
  return PRIVATE(this)->name;
}

void
NbNavigationSystem::setCamera(SoCamera * camera)
{
  PRIVATE(this)->info->setCamera(camera);
}

void
NbNavigationSystem::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->info->setViewport(vp);
}

void
NbNavigationSystem::addModeChangeCallback(NbNavigationModeChangeCB * cb, void * closure)
{
  if ( !PRIVATE(this)->modechangecbs ) {
    PRIVATE(this)->modechangecbs = new SbPList;
  }
  PRIVATE(this)->modechangecbs->append(cb);
  PRIVATE(this)->modechangecbs->append(closure);
}

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

SbBool
NbNavigationSystem::processEvent(const SoEvent * event)
{
  assert(event);
  assert(PRIVATE(this)->state);
  assert(PRIVATE(this)->info);
  NbNavigationMode * mode = PRIVATE(this)->state->getMode();
  if ( mode == NULL ) return FALSE;
  // fprintf(stderr, "NbNavigationSystem::processEvent()\n");

  SbBool retval = FALSE;
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
    // fprintf(stderr, "triggering transition %d\n", i);
    switch ( transition->type ) {
    case FINISH:
    case ABORT:
      if ( mode->processEvent(event, PRIVATE(this)->info) ) retval = TRUE;
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
	if ( mode->processEvent(event, PRIVATE(this)->info) ) retval = TRUE;
      }
      return retval;

    case STACK:
    case SWITCH:
      if ( mode->processEvent(event, PRIVATE(this)->info) ) retval = TRUE;
      mode->finish(event, PRIVATE(this)->info);
      if ( transition->type == SWITCH )
	PRIVATE(this)->state->pop();
      PRIVATE(this)->state->push(transition->mode2, event);
      mode = PRIVATE(this)->state->getMode();
      this->invokeModeChangeCallbacks();
      mode->init(event, PRIVATE(this)->info);
      if ( mode->processEvent(event, PRIVATE(this)->info) ) retval = TRUE;
      return retval;
    }
  }

  // no transition - just regular event
  if ( mode->processEvent(event, PRIVATE(this)->info) ) retval = TRUE;

  return retval;
}

void
NbNavigationSystem::addMode(NbNavigationMode * mode)
{
  assert(PRIVATE(this)->modes);
  if ( PRIVATE(this)->modes->find(mode) != -1 ) return;
  PRIVATE(this)->modes->append(mode);
}

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

SbName
NbNavigationSystem::getCurrentModeName(void) const
{
  NbNavigationMode * mode = PRIVATE(this)->state->getMode();
  if ( !mode ) return SbName("");
  return mode->getModeName();
}

const NbNavigationMode *
NbNavigationSystem::getCurrentMode(void) const
{
  return PRIVATE(this)->state->getMode();
}
 
// *************************************************************************

#undef PRIVATE
