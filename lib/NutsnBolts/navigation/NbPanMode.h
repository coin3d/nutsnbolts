#ifndef NB_PANMODE_H
#define NB_PANMODE_H

#include <NutsnBolts/navigation/NbNavigationMode.h>

class NbPanModeP;

class NB_DLL_API NbPanMode : public NbNavigationMode {
  typedef NbNavigationMode inherited;

public:
  NbPanMode(NbNavigationSystem * system, SbName name);
  ~NbPanMode(void);

protected:
  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationInfo * info);

private:
  NbPanModeP * pimpl;

}; // NbPanMode

#endif // !NB_PANMODE_H
