
#include <assert.h>
#include <stdio.h>

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoCamera.h>

#include <NutsnBolts/navigation/NbNavigationInfo.h>

// *************************************************************************

/*!
  \class NbNavigationInfo NutsnBolts/navigation/NbNavigationInfo.h
  \brief Helper class for classes implementing navigation modes.

  \ingroup navigation
*/

class NbNavigationInfoP {
public:
  NbNavigationInfoP(NbNavigationInfo * api);
  ~NbNavigationInfoP(void);

  SbViewportRegion viewport;
  SoCamera * initcamera;
  SoCamera * cameraptr;
}; // NbNavigationInfoP

NbNavigationInfoP::NbNavigationInfoP(NbNavigationInfo * api)
{
}

NbNavigationInfoP::~NbNavigationInfoP(void)
{
}

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NbNavigationInfo::NbNavigationInfo(void)
{
  PRIVATE(this) = new NbNavigationInfoP(this);
  PRIVATE(this)->initcamera = NULL;
  PRIVATE(this)->cameraptr = NULL;
}

NbNavigationInfo::~NbNavigationInfo(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

void
NbNavigationInfo::setCamera(SoCamera * camera)
{
  // fprintf(stderr, "NbNavigationInfo::setCamera(SoCamera *)\n");
  if ( PRIVATE(this)->initcamera ) {
    PRIVATE(this)->initcamera->unref();
    PRIVATE(this)->initcamera = NULL;
  }
  if ( PRIVATE(this)->cameraptr ) {
    PRIVATE(this)->cameraptr->unref();
    PRIVATE(this)->cameraptr = NULL;
  }
  if ( camera ) {
    PRIVATE(this)->initcamera = (SoCamera *) camera->copy(FALSE);
    PRIVATE(this)->initcamera->ref();
    PRIVATE(this)->cameraptr = camera;
    PRIVATE(this)->cameraptr->ref();
  }
}

SoCamera *
NbNavigationInfo::getCamera(void) const
{
  return PRIVATE(this)->cameraptr;
}

void
NbNavigationInfo::setCamera(void) const
{
  // fprintf(stderr, "NbNavigationInfo::setCamera()\n");
  if ( !PRIVATE(this)->cameraptr || !PRIVATE(this)->initcamera ) return;
  PRIVATE(this)->initcamera->copyFieldValues(PRIVATE(this)->cameraptr);}

void
NbNavigationInfo::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->viewport = vp;
}

void
NbNavigationInfo::restoreCamera(void) const
{
  // fprintf(stderr, "NbNavigationInfo::restoreCamera()\n");
  if ( PRIVATE(this)->cameraptr == NULL ) return;
  PRIVATE(this)->cameraptr->copyFieldValues(PRIVATE(this)->initcamera);
}

void
NbNavigationInfo::reorientCamera(const SbRotation & rot) const
{
  // fprintf(stderr, "NbNavigationInfo::reorientCamera()\n");
  if ( PRIVATE(this)->cameraptr == NULL ) return;

  SoCamera * cam = PRIVATE(this)->cameraptr;

  // Find global coordinates of focal point.
  SbVec3f direction;
  cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  SbVec3f focalpoint = cam->position.getValue() +
    cam->focalDistance.getValue() * direction;

  // Set new orientation value by accumulating the new rotation.
  cam->orientation = rot * cam->orientation.getValue();

  // Reposition camera so we are still pointing at the same old focal point.
  cam->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  cam->position = focalpoint - cam->focalDistance.getValue() * direction;
}

void
NbNavigationInfo::moveCamera(const SbVec3f & vec) const
{
  // fprintf(stderr, "NbNavigationInfo::moveCamera()\n");
  if ( PRIVATE(this)->cameraptr == NULL ) return;
  PRIVATE(this)->cameraptr->position =
    PRIVATE(this)->cameraptr->position.getValue() + vec;
}

SbVec2s
NbNavigationInfo::getViewportSize(void) const
{
  return PRIVATE(this)->viewport.getWindowSize();
}

float
NbNavigationInfo::getViewportAspect(void) const
{
  return PRIVATE(this)->viewport.getViewportAspectRatio();
}

// *************************************************************************

#undef PRIVATE
