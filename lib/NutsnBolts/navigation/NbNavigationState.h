#ifndef NB_NAVIGATIONSTATE_H
#define NB_NAVIGATIONSTATE_H

#include <NutsnBolts/Basic.h>

class SoEvent;
class NbNavigationMode;

class NbNavigationStateP;

class NbNavigationState {
public:
  NbNavigationState(void);
  ~NbNavigationState(void);

  void push(NbNavigationMode * mode, const SoEvent * trigger);
  void pop(void);
  void reset(void);

  NbNavigationMode * getMode(void) const;
  const SoEvent * getTrigger(void) const;

private:
  NbNavigationStateP * pimpl;

}; // NbNavigationState

#endif // !NB_NAVIGATIONSTATE_H
