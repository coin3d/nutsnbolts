#ifndef NB_NAMED_EVENT_H
#define NB_NAMED_EVENT_H

#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoSubEvent.h>
#include <Inventor/SbName.h>

#include <NutsnBolts/Basic.h>

class NB_DLL_API NbNamedEvent : public SoEvent {
  typedef SoEvent inherited;
  
  SO_EVENT_HEADER();

  NbNamedEvent(void);
  virtual ~NbNamedEvent();

  void setName(const SbName & name);
  const SbName & getName(void) const;

  static void initClass(void);

  enum Name {
    TOOLTIP,
    INIT_SIMULATOR
  };

  static SbName getName(const Name name);

 private:
  SbName name;
};


#endif // NB_NAMED_EVENT_H
