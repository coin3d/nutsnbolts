#include "NbNamedEvent.h"
#include <Inventor/C/tidbits.h>

// Currently used Names:
// - MouseButtonClick:x - mouse button click event. x can be 1, 2 or 3 
// - Tooltip - mouse button hasn't moved for some time
// - InitSimulator - called after the scene graph has been loaded/created


SO_EVENT_SOURCE(NbNamedEvent);

static SbName * tooltip_name = NULL;
static SbName * init_sim_name = NULL;

static void namedevent_cleanup(void)
{
  delete tooltip_name;
  delete init_sim_name;
}

NbNamedEvent::NbNamedEvent(void)
  : name("")
{
  this->setPosition(SbVec2s(0,0));
}

NbNamedEvent::~NbNamedEvent()
{
}

void
NbNamedEvent::initClass(void)
{
  SO_EVENT_INIT_CLASS(NbNamedEvent, SoEvent);

  tooltip_name = new SbName("Tooltip");
  init_sim_name = new SbName("InitSimulator");

  cc_coin_atexit((coin_atexit_f*) namedevent_cleanup);
}

void 
NbNamedEvent::setName(const SbName & name)
{
  this->name = name;
}

const SbName & 
NbNamedEvent::getName(void) const
{
  return this->name;
}

SbName 
NbNamedEvent::getName(const Name name)
{
  switch (name) {
  case TOOLTIP:
    return *tooltip_name;
  case INIT_SIMULATOR:
    return *init_sim_name;
  default:
    assert(0 && "unknown name");
  }
  return SbName("");
}
