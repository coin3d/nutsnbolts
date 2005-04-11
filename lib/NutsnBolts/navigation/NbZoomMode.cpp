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
#include <math.h>
#include <float.h>

#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>

#include <NutsnBolts/navigation/NbNavigationControl.h>

#include <NutsnBolts/navigation/NbZoomMode.h>

// *************************************************************************

/*!
  \class NbZoomMode NutsnBolts/navigation/NbZoomMode.h
  \brief Class that implements the zooming action.

  This class implements zooming by mouse movement.

  \ingroup navigation
*/

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)

NB_NAVIGATION_MODE_SOURCE(NbZoomMode);

void
NbZoomMode::initClass(void)
{
  NB_NAVIGATION_MODE_INIT_CLASS(NbZoomMode, NbNavigationMode);
}

/*!
  Constructor.
*/

NbZoomMode::NbZoomMode(SbName name)
  : inherited(name)
{
  // no need for a private implementation now, but we've set off room for
  // it if we ever need one in the future.
  PRIVATE(this) = NULL;
  this->set1DValueFunc(NbNavigationMode::getMouseMoveVerticalNormalizedDistance,
		       NULL);
}

/*!
  Destructor.
*/

NbZoomMode::~NbZoomMode(void)
{
}

NbNavigationMode *
NbZoomMode::clone(void) const
{
  return new NbZoomMode(this->getModeName());
}

/*!
  This method handles the zooming operation.

  Returns FALSE for unused events, and TRUE for events that are used.
*/
// FIXME: use NbNavigationControl::moveCamera() instead
SbBool
NbZoomMode::handleEvent(const SoEvent * event, const NbNavigationControl * ctrl)
{

  SoCamera * camera = ctrl->getCamera();
  if (!camera) {
    return FALSE;
  }

  if ( event->isOfType(SoMouseButtonEvent::getClassTypeId()) ) {
    SoMouseButtonEvent * mbevent = (SoMouseButtonEvent *) event;
    SoMouseButtonEvent::Button button = mbevent->getButton();
    SoButtonEvent::State state = mbevent->getState();
    if ( button == SoMouseButtonEvent::BUTTON4 ) {
      if ( state == SoButtonEvent::DOWN ) {
        ctrl->moveCamera(0.05f);
        ctrl->saveCamera();
      }
      this->finish();
      return TRUE;
    } else if ( button == SoMouseButtonEvent::BUTTON5 ) {
      if ( state == SoButtonEvent::DOWN ) {
        ctrl->moveCamera(-0.05f);
        ctrl->saveCamera();
      }
      this->finish();
      return TRUE;
    }
    return FALSE;
  }

  if (! event->isOfType(SoLocation2Event::getClassTypeId())) {
    return FALSE;
  }

  const float factor = this->get1DValue(ctrl) * 20.0f;
  float multiplicator = float(exp(factor));

  ctrl->restoreCamera();

  if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    SoOrthographicCamera * oc = (SoOrthographicCamera *) camera;
    oc->height = oc->height.getValue() * multiplicator;
  } else if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {

    const float oldfocaldist = camera->focalDistance.getValue();
    const float newfocaldist = oldfocaldist * multiplicator;

    SbVec3f direction;
    camera->orientation.getValue().multVec(SbVec3f(0, 0, -1), direction);

    const SbVec3f oldpos = camera->position.getValue();
    const SbVec3f newpos = oldpos + (newfocaldist - oldfocaldist) * -direction;

    // This catches a rather common user interface "buglet": if the
    // user zooms the camera out to a distance from origo larger than
    // what we still can safely do floating point calculations on
    // (i.e. without getting NaN or Inf values), the faulty floating
    // point values will propagate until we start to get debug error
    // messages and eventually an assert failure from core Coin code.
    //
    // With the below bounds check, this problem is avoided.
    //
    // (But note that we depend on the input argument ''diffvalue'' to
    // be small enough that zooming happens gradually. Ideally, we
    // should also check distorigo with isinf() and isnan() (or
    // inversely; isinfite()), but those only became standardized with
    // C99.)

    const float distorigo = newpos.length();
    // sqrt(FLT_MAX) == ~ 1e+19, which should be both safe for further
    // calculations and ok for the end-user and app-programmer.
    if (distorigo > float(sqrt(FLT_MAX))) {
#if 0 // debug
      fprintf(stderr, "zoomed too far (distance to origo==%f (%e))",
	      distorigo, distorigo);
#endif // debug
    }
    else {
      camera->position = newpos;
      camera->focalDistance = newfocaldist;
    }
  } else {
    // unsupported camera type - no zoom implemented
    return FALSE;
  }

  return TRUE;
}

#undef PRIVATE

// *************************************************************************
