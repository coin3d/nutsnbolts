#ifndef NB_SCRIPT_EVENT_H
#define NB_SCRIPT_EVENT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoMFString.h>
#include <Inventor/fields/SoSFNode.h>
#include <Inventor/fields/SoSFString.h>
#include <Inventor/fields/SoSFTrigger.h>
#include <NutsnBolts/Basic.h>

class NbScriptEventP;
class SoEvent;

class NB_DLL_API NbScriptEvent : public SoNode {
  typedef SoNode inherited;
  
  SO_NODE_HEADER(NbScriptEvent);

 public:
  
  NbScriptEvent(void);
  static void initClass(void);
  
  virtual void execScript(void);

  SoMFString eventName;
  SoSFNode scriptNode;
  SoSFString script;

  SoSFTrigger trigger;
  
  SbBool handle(const SoEvent * event, SoHandleEventAction * action);
  
  SbBool isClickedEvent(const SbString & name);
  SbBool isTooltipEvent(const SbName & name);
  SbBool isGlobalEvent(const SbName & name);
  SbBool isObjectPicked(const SoEvent * event, SoHandleEventAction * action);

protected:
  virtual ~NbScriptEvent();
  
private:
  void fetchParentNodes(SoHandleEventAction * action);
  friend class NbScriptEventP;
  NbScriptEventP * pimpl;
};

#endif // NB_SCRIPT_EVENT_H
