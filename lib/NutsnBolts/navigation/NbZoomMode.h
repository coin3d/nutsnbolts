#ifndef NB_ZOOMMODE_H
#define NB_ZOOMMODE_H

#include <NutsnBolts/navigation/NbNavigationMode.h>

class NbZoomModeP;

class NB_DLL_API NbZoomMode : public NbNavigationMode {
  typedef NbNavigationMode inherited;

public:
  NbZoomMode(NbNavigationSystem * system, SbName name);
  virtual ~NbZoomMode(void);

protected:
  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationInfo * info);

private:
  NbZoomModeP * pimpl;

}; // NbZoomMode

#endif // !NB_ZOOMMODE_H
