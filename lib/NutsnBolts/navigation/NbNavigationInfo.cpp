/**************************************************************************\
 *
 *  This file is part of the SIM Nuts'n'Bolts extension library for Coin.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License ("GPL") version 2
 *  as published by the Free Software Foundation.  See the file COPYING
 *  at the root directory of this source distribution for additional
 *  information about the GNU GPL.
 *
 *  For using SIM Nuts'n'Bolts with software that can not be combined with
 *  the GNU GPL, and for taking advantage of the additional benefits of
 *  our support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion AS, Bygdøy allé 5, N-0257 Oslo, NORWAY. (www.sim.no)
 *
\**************************************************************************/

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

  This class contains helper functions for implementing navigation modes.

  Consider this class internal unless you are implementing your own
  navigation modes.

  \ingroup navigation
*/

// *************************************************************************

class NbNavigationInfoP {
public:
  NbNavigationInfoP(NbNavigationInfo * api);
  ~NbNavigationInfoP(void);

  SbViewportRegion viewport;
  SoCamera * initcamera;
  SoCamera * cameraptr;
}; // NbNavigationInfoP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/

NbNavigationInfo::NbNavigationInfo(void)
{
  PRIVATE(this) = new NbNavigationInfoP(this);
  PRIVATE(this)->initcamera = NULL;
  PRIVATE(this)->cameraptr = NULL;
}

/*!
  Destructor.
*/

NbNavigationInfo::~NbNavigationInfo(void)
{
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Sets the pointer to the camera that is to be manipulated.

  \sa getCamera
*/

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

/*!
  Returns the pointer the the camera that is to be manipulated.

  \sa setCamera
*/

SoCamera *
NbNavigationInfo::getCamera(void) const
{
  return PRIVATE(this)->cameraptr;
}

/*!
  This method sets the viewport information.
*/

void
NbNavigationInfo::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->viewport = vp;
}

/*!
  This method <i>sets</i> the camera to its current position.
  You can later restore the camera back to this position by calling
  restoreCamera().

  \sa restoreCamera
*/

void
NbNavigationInfo::setCamera(void) const
{
  // fprintf(stderr, "NbNavigationInfo::setCamera()\n");
  if ( !PRIVATE(this)->cameraptr || !PRIVATE(this)->initcamera ) return;
  PRIVATE(this)->initcamera->copyFieldValues(PRIVATE(this)->cameraptr);
}

/*!
  This method restores the camera back to its last <i>set</i> position,
  or its initial position if setCamera() has not been used yet.
*/

void
NbNavigationInfo::restoreCamera(void) const
{
  // fprintf(stderr, "NbNavigationInfo::restoreCamera()\n");
  if ( PRIVATE(this)->cameraptr == NULL ) return;
  PRIVATE(this)->cameraptr->copyFieldValues(PRIVATE(this)->initcamera);
}

/*!
  This method reorients the camera according to the given rotation.

  \sa setCamera, restoreCamera, moveCamera
*/

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

/*!
  This method moves the camera.
*/

void
NbNavigationInfo::moveCamera(const SbVec3f & vec) const
{
  // fprintf(stderr, "NbNavigationInfo::moveCamera()\n");
  if ( PRIVATE(this)->cameraptr == NULL ) return;
  PRIVATE(this)->cameraptr->position =
    PRIVATE(this)->cameraptr->position.getValue() + vec;
}

/*!
  Returns the size of the viewport in pixels.
*/

SbVec2s
NbNavigationInfo::getViewportSize(void) const
{
  return PRIVATE(this)->viewport.getWindowSize();
}

/*!
  Returns the aspect ratio of the viewport - width divided by height.
*/

float
NbNavigationInfo::getViewportAspect(void) const
{
  return PRIVATE(this)->viewport.getViewportAspectRatio();
}

#undef PRIVATE

// *************************************************************************
// private implementation

NbNavigationInfoP::NbNavigationInfoP(NbNavigationInfo * api)
{
}

NbNavigationInfoP::~NbNavigationInfoP(void)
{
}

// *************************************************************************
