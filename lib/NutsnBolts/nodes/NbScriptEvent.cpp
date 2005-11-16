#include "NbScriptEvent.h"
#include "NbEventNode.h"
#include <NutsnBolts/misc/NbNamedEvent.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/SoPickedPoint.h>
#include <assert.h>

/*!
  \class NbScriptEvent g2kdb/scenario/NbScriptEvent.h
  \brief The NbScriptEvent class enables you to trigger scripts based on event names.

  See NbNamedEvent for information about how events are named. For
  MouseClickEvents, this node also checks for intersections before
  triggering the script. This node should always be added to the event
  field of a NbEventNode node, and not used anywhere else.

  TODO: doc
*/

SO_NODE_SOURCE(NbScriptEvent);

// not sure if these are needed in this node...
// A pointer to the currently executing event node.
static NbScriptEvent * scriptevent_currenteventnode;
static NbEventNode * scriptevent_currenteventnodeparent;

void 
NbScriptEvent::initClass(void)
{
  static int first = 1;
  if (first) {
    first = 0;
    SO_NODE_INIT_CLASS(NbScriptEvent, SoNode, "SoNode");
  }

  scriptevent_currenteventnode = NULL;
  scriptevent_currenteventnodeparent = NULL;
}

class NbScriptEventP {
public:
  NbScriptEventP(NbScriptEvent * master) : master(master) { }  
  NbScriptEvent * master;
};

#define PRIVATE(obj) obj->pimpl

/*!
  Constructor.
*/
NbScriptEvent::NbScriptEvent(void)
{
  SO_NODE_CONSTRUCTOR(NbScriptEvent);  

  SO_NODE_ADD_FIELD(eventName, (""));
  eventName.setNum(0);
  eventName.setDefault(TRUE);
  SO_NODE_ADD_FIELD(scriptNode, (NULL));
  SO_NODE_ADD_FIELD(script, (""));
  SO_NODE_ADD_FIELD(trigger, ());
}

/*!
  Destructor.
*/
NbScriptEvent::~NbScriptEvent()
{
}

/*!
  Handle a single event. Will trigger if the event name matches one
  of the names in the eventName field.
*/
SbBool 
NbScriptEvent::handle(const SoEvent * event, SoHandleEventAction * action)
{
  const int num = this->eventName.getNum();

  SbString name = "";

  if (event->isOfType(SoKeyboardEvent::getClassTypeId())) {
    name += "Key";
    
    if (SoKeyboardEvent::isKeyPressEvent(event, SoKeyboardEvent::ANY)) {
      name += "Press";
    }
    else {
      name += "Release";
    }
    name += ":";
    name += ((SoKeyboardEvent*) event)->getPrintableCharacter();
  }
  else if (event->isOfType(SoMouseButtonEvent::getClassTypeId())) {
    SoMouseButtonEvent * me = (SoMouseButtonEvent*) event;
    name += "MouseButton";
    if (SoMouseButtonEvent::isButtonPressEvent(event,
                                               SoMouseButtonEvent::ANY)) {
      name += "Press";
    }
    else {
      name += "Release";
    }
    name += ":";
    name.addIntString(me->getButton());    
  }
  else if (event->isOfType(NbNamedEvent::getClassTypeId())) {
    name = ((NbNamedEvent*) event)->getName();
  }
  // FIXME: name other Coin events as well
  SbName quickname(name.getString());
  
  const SbString * eventnames = this->eventName.getValues(0);
  SbBool checkpicked = this->isClickedEvent(name) || this->isTooltipEvent(quickname);
  
  int once = TRUE;
  for (int i = 0; i < num; i++) {
    if (eventnames[i] == name) {
      if (!checkpicked || (action && this->isObjectPicked(event, action))) {
        
        if (once) {         
          // Set ptr to this node before executing script.  
          scriptevent_currenteventnode = this; 
           
          if (action && action->getCurPath()) {
            once = FALSE;
            this->fetchParentNodes(action);                     
          }
        }

        this->execScript();

        // only handle event if other nodes shouldn't get it
        return !this->isGlobalEvent(quickname);
      }
    }
  }
  
  return FALSE;
}

SbBool 
NbScriptEvent::isClickedEvent(const SbString & name)
{
  if (name.find("MouseButtonClick") >= 0) return TRUE;
  return FALSE;
}

SbBool 
NbScriptEvent::isTooltipEvent(const SbName & name)
{
  if (name == NbNamedEvent::getName(NbNamedEvent::TOOLTIP)) return TRUE;
  return FALSE;
}

SbBool 
NbScriptEvent::isGlobalEvent(const SbName & name)
{
  if (name == NbNamedEvent::getName(NbNamedEvent::INIT_SIMULATOR)) return TRUE;
  return FALSE;
}

SbBool 
NbScriptEvent::isObjectPicked(const SoEvent * event,
                              SoHandleEventAction * action)
{
  const SoPickedPoint * pp = action->getPickedPoint();
    
  if (pp && pp->getPath()->containsPath(action->getCurPath())) return TRUE;
  return FALSE;
}

void
NbScriptEvent::fetchParentNodes(SoHandleEventAction * action)     
{
  SoFullPath * path = (SoFullPath*) action->getCurPath();
  SoNode * tail = path->getTail();
  
  // Set ptr to parent node
  scriptevent_currenteventnodeparent = (NbEventNode *) tail; 
}

void 
NbScriptEvent::execScript(void)
{
  // touch trigger field to make it possible to attach a field sensor
  // to exec script
  this->trigger.touch();
}



