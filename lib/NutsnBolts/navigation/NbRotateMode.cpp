
#include <assert.h>
#include <stdio.h>

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbSphere.h>
#include <Inventor/SbViewVolume.h>
#include <Inventor/events/SoEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/projectors/SbSphereSheetProjector.h>

#include <NutsnBolts/navigation/NbNavigationInfo.h>

#include <NutsnBolts/navigation/NbRotateMode.h>

// *************************************************************************

/*!
  \class NbRotateMode NutsnBolts/navigation/NbRotateMode.h
  \brief Class that implements the rotate mode.

  \ingroup navigation
*/

class NbRotateModeP {
public:
  NbRotateModeP(NbRotateMode * api) {
    this->projector = NULL;
  }
  ~NbRotateModeP(void) {
    if ( this->projector ) {
      delete this->projector;
      this->projector = NULL;
    }
  }

public:
  SbSphereSheetProjector * projector;

}; // NbRotateModeP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NbRotateMode::NbRotateMode(NbNavigationSystem * system, SbName modename)
  : inherited(system, modename)
{
  PRIVATE(this) = new NbRotateModeP(this);
}

NbRotateMode::~NbRotateMode(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

SbBool
NbRotateMode::handleEvent(const SoEvent * event, const NbNavigationInfo * info)
{
  fprintf(stderr, "NbRotateMode::handleEvent() %s\n",
	  this->getModeName().getString());

  if ( !event->isOfType(SoLocation2Event::getClassTypeId()) ) {
    return FALSE;
  }

  SbVec2f initpos(this->getInitialNormalizedPosition(info));
  SbVec2f currentpos(this->getCurrentNormalizedPosition(info));

  if ( PRIVATE(this)->projector == NULL ) {
    PRIVATE(this)->projector =
      new SbSphereSheetProjector(SbSphere(SbVec3f(0, 0, 0), 0.8f));
    SbViewVolume volume;
    volume.ortho(-1, 1, -1, 1, -1, 1);
    PRIVATE(this)->projector->setViewVolume(volume);
  }

  info->restoreCamera();

  PRIVATE(this)->projector->project(initpos);
  SbRotation rot;
  PRIVATE(this)->projector->projectAndGetRotation(currentpos, rot);
  rot.invert();

  info->reorientCamera(rot);
  return TRUE;
}

void
NbRotateMode::abort(const SoEvent * event, const NbNavigationInfo * info)
{
  info->restoreCamera();
  inherited::abort(event, info);
}

#undef PRIVATE
