#ifndef NB_IDLEMODE_H
#define NB_IDLEMODE_H

#include <NutsnBolts/navigation/NbNavigationMode.h>

class NbIdleModeP;

class NB_DLL_API NbIdleMode : public NbNavigationMode {
  typedef NbNavigationMode inherited;

public:
  NbIdleMode(NbNavigationSystem * system, SbName modename);
  virtual ~NbIdleMode(void);

protected:
  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationInfo * info);

private:
  NbIdleModeP * pimpl;

}; // NbIdleMode

#endif // !NB_IDLEMODE_H
