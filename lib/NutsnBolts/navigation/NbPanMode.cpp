
#include <stdio.h>
#include <assert.h>

#include <Inventor/SbLine.h>
#include <Inventor/SbPlane.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/nodes/SoCamera.h>

#include <NutsnBolts/navigation/NbNavigationInfo.h>

#include <NutsnBolts/navigation/NbPanMode.h>

// *************************************************************************

/*!
  \class NbPanMode NutsnBolts/navigation/NbPanMode.h
  \brief Class that implements the panning mode.

  \ingroup navigation
*/

/*!
  The constructor.

  \a system The navigfation system it belongs to.
  \a name the name of the navigation mode.
*/

NbPanMode::NbPanMode(NbNavigationSystem * system, SbName name)
  : inherited(system, name)
{
  // we don't have the need for a private implementation, but we have
  // set off space for one if we ever need one in the future.
  this->pimpl = NULL;
}

/*!
  The destructor.
*/

NbPanMode::~NbPanMode(void)
{
}

/*!
  This method handles the panning operation.
*/

SbBool
NbPanMode::handleEvent(const SoEvent * event, const NbNavigationInfo * info)
{
  if ( ! event->isOfType(SoLocation2Event::getClassTypeId()) ) {
    return FALSE;
  }

  SoCamera * camera = info->getCamera();
  if ( !camera ) {
    return FALSE;
  }

  // Find projection points for the current and the reference mouse
  // coordinates.
  SbViewVolume vv = camera->getViewVolume(info->getViewportAspect());
  SbPlane panningplane = vv.getPlane(camera->focalDistance.getValue());
  SbLine line;
  SbVec3f current_planept, initial_planept;
  vv.projectPointToLine(this->getCurrentNormalizedPosition(info), line);
  panningplane.intersect(line, current_planept);
  vv.projectPointToLine(this->getInitialNormalizedPosition(info), line);
  panningplane.intersect(line, initial_planept);

  info->restoreCamera();
  info->moveCamera(initial_planept - current_planept);

  return TRUE;
}
