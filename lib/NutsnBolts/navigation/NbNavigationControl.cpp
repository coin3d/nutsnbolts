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
#include <Inventor/SbRotation.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoRayPickAction.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>

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

class NbNavigationControlP {
public:
  NbNavigationControlP(NbNavigationControl * api);
  ~NbNavigationControlP(void);

  SbViewportRegion viewport;
  SoCamera * initcamera;
  SoCamera * cameraptr;

  SoNode * scenegraph;
}; // NbNavigationControlP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

/*!
  Constructor.
*/

NbNavigationControl::NbNavigationControl(void)
{
  PRIVATE(this) = new NbNavigationControlP(this);
  PRIVATE(this)->initcamera = NULL;
  PRIVATE(this)->cameraptr = NULL;
  PRIVATE(this)->scenegraph = NULL;
}

/*!
  Destructor.
*/

NbNavigationControl::~NbNavigationControl(void)
{

  if (PRIVATE(this)->initcamera) {
    PRIVATE(this)->initcamera->unref();
    PRIVATE(this)->initcamera = NULL;
  }
  if (PRIVATE(this)->cameraptr) {
    PRIVATE(this)->cameraptr->unref();
    PRIVATE(this)->cameraptr = NULL;
  }
  this->setSceneGraph(NULL);
  delete PRIVATE(this);
  PRIVATE(this) = NULL;
}

/*!
  Sets the scene graph.  Needed for certain navigation modes that for
  instance perform picking.

  \sa getSceneGraph
*/

void
NbNavigationControl::setSceneGraph(SoNode * scenegraph)
{
  if (PRIVATE(this)->scenegraph) {
    PRIVATE(this)->scenegraph->unref();
    PRIVATE(this)->scenegraph = NULL;
  }
  if (scenegraph) {
    PRIVATE(this)->scenegraph = scenegraph;
    PRIVATE(this)->scenegraph->ref();
  }
}

/*!
  Returns the set scene graph.

  \sa setSceneGraph
*/

SoNode *
NbNavigationControl::getSceneGraph(void) const
{
  return PRIVATE(this)->scenegraph;
}

/*!
  Sets the pointer to the camera that is to be manipulated.

  \sa getCamera
*/

void
NbNavigationControl::setCamera(SoCamera * camera)
{
  // fprintf(stderr, "NbNavigationControl::setCamera(SoCamera *)\n");
  if (PRIVATE(this)->initcamera) {
    PRIVATE(this)->initcamera->unref();
    PRIVATE(this)->initcamera = NULL;
  }
  if (PRIVATE(this)->cameraptr) {
    PRIVATE(this)->cameraptr->unref();
    PRIVATE(this)->cameraptr = NULL;
  }
  if (camera) {
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
NbNavigationControl::getCamera(void) const
{
  return PRIVATE(this)->cameraptr;
}

/*!
  This method <i>saves</i> the current camera position and orientation.
  You can later restore the camera back to this position by calling
  restoreCamera().

  \sa restoreCamera
*/

void
NbNavigationControl::saveCamera(void) const
{
  // fprintf(stderr, "NbNavigationControl::setCamera()\n");
  if (!PRIVATE(this)->cameraptr || !PRIVATE(this)->initcamera) return;
  PRIVATE(this)->initcamera->copyFieldValues(PRIVATE(this)->cameraptr);
}

/*!
  This method restores the camera back to its last <i>set</i> position,
  or its initial position if saveCamera() has not been used yet.

  \sa saveCamera
*/

void
NbNavigationControl::restoreCamera(void) const
{
  // fprintf(stderr, "NbNavigationControl::restoreCamera()\n");
  if (PRIVATE(this)->cameraptr == NULL) return;
  PRIVATE(this)->cameraptr->copyFieldValues(PRIVATE(this)->initcamera);
}

SbBool
NbNavigationControl::pick(SbVec2s pos, SbVec3f & pickpos) const
{
  return FALSE;
}

/*!
  This method reorients the camera according to the given rotation.

  \sa saveCamera, restoreCamera, moveCamera
*/

void
NbNavigationControl::reorientCamera(const SbRotation & rot) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  // Find global coordinates of focal point.
  SbVec3f direction;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  SbVec3f focalpoint = camera->position.getValue() +
    camera->focalDistance.getValue() * direction;

  // Set new orientation value by accumulating the new rotation.
  camera->orientation = rot * camera->orientation.getValue();

  // Reposition camera so we are still pointing at the same old focal point.
  camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);
  camera->position = focalpoint - camera->focalDistance.getValue() * direction;
}

/*!
*/

void
NbNavigationControl::reorientCamera(const SbVec3f & pointat) const
{
}

/*!
*/

void
NbNavigationControl::pitchCamera(float angle) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  SbMatrix camerarot;
  camerarot.setRotate(camera->orientation.getValue());

  SbMatrix pitchmat;
  pitchmat.setRotate(SbRotation(SbVec3f(0.0f, 1.0f, 0.0f), angle));
  camerarot.multLeft(pitchmat);
  camera->orientation = SbRotation(camerarot);
}

/*!
*/

void
NbNavigationControl::yawCamera(float angle) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  SbMatrix camerarot;
  camerarot.setRotate(camera->orientation.getValue());

  SbMatrix yawmat;
  yawmat.setRotate(SbRotation(SbVec3f(1.0f, 0.0f, 0.0f), angle));
  camerarot.multLeft(yawmat);
  camera->orientation = SbRotation(camerarot);
}

/*!
*/

void
NbNavigationControl::rollCamera(float angle) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  SbMatrix camerarot;
  camerarot.setRotate(camera->orientation.getValue());

  SbMatrix rollmat;
  rollmat.setRotate(SbRotation(SbVec3f(0.0f, 0.0f, 1.0f), angle));
  camerarot.multLeft(rollmat);
  camera->orientation = SbRotation(camerarot);
}

/*!
  This method moves the camera.

  \sa reorientCamera, saveCamera, restoreCamera
*/

void
NbNavigationControl::moveCamera(const SbVec3f & distance) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  camera->position = camera->position.getValue() + distance;
}

/*!
*/

void
NbNavigationControl::moveCamera(float factor, SbBool keepfocalpoint) const
{
  SoCamera * camera = this->getCamera();
  if (camera == NULL) return;

  SbVec3f vec;
  camera->orientation.getValue().multVec(SbVec3f(0, 0, 1), vec);
  vec.normalize();
  SbVec3f point = camera->position.getValue() +
    (vec * camera->focalDistance.getValue());
  camera->position = camera->position.getValue() + (vec * factor);

  if (keepfocalpoint) {
    vec = point - camera->position.getValue();
    camera->focalDistance = vec.length();
  }
}

/*!
  This method sets the viewport information.
*/

void
NbNavigationControl::setViewport(const SbViewportRegion & vp)
{
  PRIVATE(this)->viewport = vp;
}

/*!
  Returns the size of the viewport in pixels.
*/

SbVec2s
NbNavigationControl::getViewportSize(void) const
{
  return PRIVATE(this)->viewport.getWindowSize();
}

/*!
  Returns the aspect ratio of the viewport - width divided by height.
*/

float
NbNavigationControl::getViewportAspect(void) const
{
  return PRIVATE(this)->viewport.getViewportAspectRatio();
}

#undef PRIVATE

// *************************************************************************
// private implementation

NbNavigationControlP::NbNavigationControlP(NbNavigationControl * api)
{
}

NbNavigationControlP::~NbNavigationControlP(void)
{
}

// *************************************************************************
