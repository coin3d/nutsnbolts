#ifndef NB_EVENT_NODE_H
#define NB_EVENT_NODE_H

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSubKit.h>
#include <Inventor/fields/SoMFNode.h>
#include <NutsnBolts/Basic.h>

class NbEventNodeP;
class SoEvent;
class SoSensor;

class NB_DLL_API NbEventNode : public SoBaseKit {
  typedef SoBaseKit inherited;
  
  SO_KIT_HEADER(NbEventNode);
  SO_KIT_CATALOG_ENTRY_HEADER(topSwitch);
  SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
  
 public:
  
  NbEventNode(void);
  static void initClass(void);
  
  SoMFNode event;

  virtual void jumpTo(void);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void write(SoWriteAction * action);
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  virtual SbBool handleEvent(const SoEvent * event, SoHandleEventAction * action);
  
protected:
  virtual ~NbEventNode();

private:
  friend class NbEventNodeP;
  NbEventNodeP * pimpl;
};


#endif // NB_EVENT_NODE_H
