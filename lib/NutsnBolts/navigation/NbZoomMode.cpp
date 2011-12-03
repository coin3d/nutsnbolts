/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/fields/SoSFVec3d.h>
#include <Inventor/SbVec3d.h>

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
    if (button == SoMouseButtonEvent::BUTTON4) {  // Zoom in
      if (state == SoButtonEvent::DOWN) {
        if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
          SoOrthographicCamera * oc = (SoOrthographicCamera *) camera;
          oc->height = oc->height.getValue() * 0.95f;
        }
        ctrl->moveCamera(0.05f);
        ctrl->saveCamera();
      }
      this->finish();
      return TRUE; 
    } else if (button == SoMouseButtonEvent::BUTTON5) {  // Zoom out
      if (state == SoButtonEvent::DOWN) {
        if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
          SoOrthographicCamera * oc = (SoOrthographicCamera *) camera;
          oc->height = oc->height.getValue() * 1.05f;
        }
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

  const float factor = this->get1DValue(ctrl) * 15.0f;
  float multiplicator = float(exp(factor));

  ctrl->restoreCamera();

  if (camera->isOfType(SoOrthographicCamera::getClassTypeId())) {
    SoOrthographicCamera * oc = (SoOrthographicCamera *) camera;
    oc->height = oc->height.getValue() * multiplicator;
  } else if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
#if 0
    // bogus
    fprintf(stderr, "NbZoomMode::handleEvent()\n");
    fprintf(stderr, "factor = %f\n", factor);
    fprintf(stderr, "multiplicator = %f\n", multiplicator);
    ctrl->moveCamera(1.0f+multiplicator);
    return TRUE;
#endif

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

    SoType utmcamtype = SoType::fromName("UTMCamera");
  if (utmcamtype != SoType::badType() &&
      camera->isOfType(utmcamtype)) {
    
      SoSFVec3d * utmposfield = (SoSFVec3d *) camera->getField("utmposition");
      SbVec3d camposd;
      camposd.setValue(camera->position.getValue());
      utmposfield->setValue(utmposfield->getValue() + camposd);
      camera->position.setValue(0.0f, 0.0f, 0.0f);
    }
  } else {
    // unsupported camera type - no zoom implemented
    return FALSE;
  }

  return TRUE;
}

#undef PRIVATE

// *************************************************************************
