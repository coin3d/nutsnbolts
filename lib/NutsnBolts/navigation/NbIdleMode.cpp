
#include <assert.h>
#include <stdio.h>

#include <NutsnBolts/navigation/NbIdleMode.h>

// *************************************************************************

/*!
  \class NbIdleMode NutsnBolts/navigation/NbIdleMode.h
  \brief Class implementing an idle mode.

  This navigation mode does nothing.

  \ingroup navigation
*/


NbIdleMode::NbIdleMode(NbNavigationSystem * system, SbName modename)
  : inherited(system, modename)
{
  this->pimpl = NULL;
}

NbIdleMode::~NbIdleMode(void)
{
}

SbBool
NbIdleMode::handleEvent(const SoEvent * event, const NbNavigationInfo * info)
{
  //fprintf(stderr, "NbIdleMode::handleEvent() %s\n",
  //	  this->getModeName().getString());
  return FALSE;
}

