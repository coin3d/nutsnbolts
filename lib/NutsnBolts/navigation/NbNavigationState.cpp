
#include <Inventor/lists/SbList.h>

#include <NutsnBolts/navigation/NbNavigationState.h>

// *************************************************************************

/*!
  \class NbNavigationState NutsnBolts/navigation/NbNavigationState.h
  \brief Internal class.

  This class is only used by NbNavigationSystem to track the current
  navigation state (current mode).

  \ingroup navigation
*/


class NbNavigationStateP {
public:
  NbNavigationStateP(NbNavigationState * api);

  SbList<NbNavigationMode *> modestack;
  SbList<const SoEvent *> triggerstack;
};

NbNavigationStateP::NbNavigationStateP(NbNavigationState * api)
{
}

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NbNavigationState::NbNavigationState(void)
{
  PRIVATE(this) = new NbNavigationStateP(this);
}

NbNavigationState::~NbNavigationState(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

void
NbNavigationState::push(NbNavigationMode * mode, const SoEvent * trigger)
{
  PRIVATE(this)->modestack.append(mode);
  PRIVATE(this)->triggerstack.append(trigger);
}

void
NbNavigationState::pop(void)
{
  PRIVATE(this)->modestack.pop();
  PRIVATE(this)->triggerstack.pop();
}

void
NbNavigationState::reset(void)
{
  PRIVATE(this)->modestack.truncate(0);
  PRIVATE(this)->triggerstack.truncate(0);
}

NbNavigationMode *
NbNavigationState::getMode(void) const
{
  if ( PRIVATE(this)->modestack.getLength() == 0 ) return NULL;
  return PRIVATE(this)->modestack[PRIVATE(this)->modestack.getLength()-1];
}

const SoEvent *
NbNavigationState::getTrigger(void) const
{
  if ( PRIVATE(this)->modestack.getLength() == 0 ) return NULL;
  return PRIVATE(this)->triggerstack[PRIVATE(this)->triggerstack.getLength()-1];
}

// *************************************************************************

#undef PRIVATE
