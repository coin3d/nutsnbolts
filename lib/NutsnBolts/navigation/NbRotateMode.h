#ifndef NB_ROTATEMODE_H
#define NB_ROTATEMODE_H

#include <NutsnBolts/navigation/NbNavigationMode.h>

class NbRotateModeP;

class NB_DLL_API NbRotateMode : public NbNavigationMode {
  typedef NbNavigationMode inherited;

public:
  NbRotateMode(NbNavigationSystem * system, SbName modename);
  virtual ~NbRotateMode(void);

protected:
  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationInfo * info);

  virtual void abort(const SoEvent * event, const NbNavigationInfo * info);

private:
  NbRotateModeP * pimpl;

}; // NbRotateMode

#endif // !NB_ROTATEMODE_H
