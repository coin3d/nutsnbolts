
#include "NbEventNode.h"
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include "NbScriptEvent.h"

/*!
  \class NbEventNode NbEventNode.h
  \brief The NbEventNode is a container for nodes which trigger scripts on events

  \todo more doc
*/

SO_KIT_SOURCE(NbEventNode);


class NbEventNodeP {
public:
  NbEventNodeP(NbEventNode * master) 
    : master(master)
  {}
  
  NbEventNode * master;
};

// Doc in parent
void 
NbEventNode::initClass(void)
{
  static int first = 1;
  if (first) {
    first = 0;
    SO_KIT_INIT_CLASS(NbEventNode, SoBaseKit, "BaseKit");
  }
}

#define PRIVATE(obj) obj->pimpl

/*!
  Constructor
*/
NbEventNode::NbEventNode(void)
{
  PRIVATE(this) = new NbEventNodeP(this);
  SO_KIT_CONSTRUCTOR(NbEventNode);

  SO_KIT_ADD_FIELD(event, (NULL));
  this->event.setNum(0);
  this->event.setDefault(TRUE);

  SO_KIT_ADD_CATALOG_ENTRY(topSwitch, SoSwitch, FALSE, this, "", TRUE);
  SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, topSwitch, "", TRUE);
  SO_KIT_INIT_INSTANCE();
  this->setUpConnections(TRUE, TRUE);

  SoSwitch * sw = (SoSwitch*) this->getAnyPart("topSwitch", TRUE);
  sw->whichChild = -3;
}

/*!
  Destructor
*/
NbEventNode::~NbEventNode()
{
  delete PRIVATE(this);
}


// Doc in parent
void 
NbEventNode::write(SoWriteAction * action)
{
  inherited::write(action);
}

// Doc in parent
SbBool 
NbEventNode::readInstance(SoInput * in, unsigned short flags)
{
  SbBool ret = inherited::readInstance(in, flags);
  return ret;
}

// Doc in parent
void 
NbEventNode::handleEvent(SoHandleEventAction * action)
{
  inherited::handleEvent(action);
  
  if (!action->isHandled()) {
    const SoEvent * e = action->getEvent();
    if (this->handleEvent(e, action)) {
      action->setHandled();
    }
  }
}

/*!
  Handle a single event. This method will loop through the events
  in the event field and call NbScriptEvent::handle() for
  each instance.
*/
SbBool 
NbEventNode::handleEvent(const SoEvent * event, SoHandleEventAction * action)
{
  const int n = this->event.getNum();
  if (n == 0) return FALSE;
  
  NbScriptEvent ** sevents = 
    (NbScriptEvent**) this->event.getValues(0);
  
  for (int i = 0; i < n; i++) {
    if (sevents[i]->handle(event, action)) return TRUE;
  }
  return FALSE;
}

void 
NbEventNode::jumpTo(void)
{
#if 0 // FIXME
  SoCamera * cam = (SoCamera*) currviewer->getCamera();
  
  SoSearchAction sa;
  sa.setSearchingAll(TRUE);
  sa.setNode(this);
  sa.setInterest(SoSearchAction::FIRST);
  sa.apply(currviewer->getSceneGraph());

  SoFullPath * path = (SoFullPath*) sa.getPath();
  if (path) {
    // SoCamera::viewBoundingBox() will change this, we don't want that
    float oldnear = cam->nearDistance.getValue();
    float oldfar = cam->farDistance.getValue();
    float oldfocal = cam->focalDistance.getValue();

    path->ref();
    sa.reset();
    if (cam->isOfType(UTMCamera::getClassTypeId())) {
      sa.setSearchingAll(TRUE);
      sa.setInterest(SoSearchAction::LAST);
      sa.setType(UTMPosition::getClassTypeId());
      
      sa.apply(path);
      if (sa.getPath()) {
        SoFullPath * p = (SoFullPath*) sa.getPath();
        UTMPosition * utm = (UTMPosition*) p->getTail();  
        ((UTMCamera*)cam)->utmposition = utm->utmposition;
      }
      else {
        myfprintf(stderr,"No UTMPosition node found in jumpTo()!\n");
      }
    }
    cam->viewAll(path, currviewer->getViewportRegion());
    
    cam->nearDistance = oldnear;
    cam->farDistance = oldfar;
    cam->focalDistance = oldfocal;

    if (cam->isOfType(UTMCamera::getClassTypeId())) {
      // move from position to utmposition and set position to (0,0,0)
      UTMCamera * utm = (UTMCamera*) cam;
      SbVec3d utmpos = utm->utmposition.getValue();
      SbVec3d tmp;
      tmp.setValue(utm->position.getValue());
      utmpos += tmp;
      utm->position = SbVec3f(0.0f, 0.0f, 0.0f);
      utm->utmposition = utmpos;
    }
    path->unref();
  }
#endif
}


#undef PRIVATE

