#ifndef NB_NAVIGATIONVEHICLE_H
#define NB_NAVIGATIONVEHICLE_H

#include <NutsnBolts/Basic.h>

class NbNavigationVehicleP;

class NB_DLL_API NbNavigationVehicle {
public:
  NbNavigationVehicle(void);
  virtual ~NbNavigationVehicle(void);

private:
  NbNavigationVehicleP * pimpl;

}; // NbNavigationVehicle

#endif // !NB_NAVIGATIONVEHICLE_H
