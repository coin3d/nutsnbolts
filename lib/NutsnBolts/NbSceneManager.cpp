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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <Inventor/SbBox.h>
#include <Inventor/SbXfBox3f.h>
#include <Inventor/SbMatrix.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoPolygonOffsetElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/nodes/SoInfo.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/SoFullPath.h>
#include <Inventor/system/gl.h>
#include <Inventor/sensors/SoNodeSensor.h>

#include <NutsnBolts/NutsnBolts.h>
#include <NutsnBolts/NbSceneManager.h>
#include <NutsnBolts/nodes/NbViewerNavigationMode.h>
#include <NutsnBolts/navigation/NbNavigationSystem.h>

// *************************************************************************

/*!
  \class NbSceneManager NutsnBolts/NbSceneManager.h
  \brief Extends the SoSceneManager class with more high-level
  viewer-related functionality.

  The NbSceneManager class extends the SoSceneManger class with
  functionality that relates to scene graphs and rendering on a higher
  level.  By using a scene manager of this type instead of the
  standard SoSceneManager, you can easily get more advanced viewer
  functionality connected to a custom GL context you've created
  yourself instead of having to resort to one of SoQt/SoWin/SoXts
  viewer components.

  Currently, the functionality this class adds to the SoSceneManager
  can be categorized into three groups:

  One set of functions lets you control the <b>rendering style</b>.
  These are setRenderMode(), setTexturesEnabled(), and
  setWireframeOverlayColor().

  Another set of functions implements support for various <b>stereo
  rendering</b> techniques.  These are setStereoMode() and
  setStereoOffset().

  The third group of functions is for incorporating a system for
  handling/intercepting SoEvent objects for doing <b>user-interactive
  camera navigation</b>.  These are setNavigationState(),
  setNavigationSystem(), setAutoClipping(), and setNearPlaneValue().
  The first two controls how user events control camera navigation.
  The latter two control how camera movements / scene changes affects
  the clipping planes the camera uses.  Moving the camera, you will
  often need the clipping planes to get updated...

  You will also notice that all the other virtual functions in the
  SoSceneManager class are overridden.  This is done because some of
  them are modified to implement the above mentioned functionality,
  and the rest of them might be necessary to override later when more
  functionality is added, and we would like to be able to do that
  without breaking binary compatibility and having to bump the major
  version of the library for the next release.

  \ingroup misc
  \ingroup navigation
*/

/*!
  \enum NbSceneManager::RenderMode

  This enum defines the various rendering styles that are supported by the
  NbSceneManager class.
*/

/*!
  \enum NbSceneManager::AS_IS

  This rendering mode will cause the scene graph to be rendered as is
  described by the scene graph.  Nothing special is done with the
  rendering.
*/

/*!
  \enum NbSceneManager::WIREFRAME

  Wireframe mode rendering will render lines instead of polygons, and
  the model will be see-through.
*/

/*!
  \enum NbSceneManager::POINTS

  Points rendering will only render points at all the used vertices in
  the model.  The model will be rendered see-through.
*/

/*!
  \enum NbSceneManager::WIREFRAME_OVERLAY

  This rendering mode combines NbSceneManager::AS_IS rendering with
  NbSceneManager::WIREFRAME rendering.  Lines will be drawn along all
  the edges in the model, on top of the ordinary model.  Two rendering
  passes is needed for this feature.  The color of the overlayed lines
  can be controlled with setWireframeOverlayColor().
*/

/*!
  \enum NbSceneManager::HIDDEN_LINE

  This rendering mode is like NbSceneManager::WIREFRAME, except that
  polygons will not be see-through.  Only the edges you actually see
  will be displayed.  Two rendering passes is used for this style.
*/

/*!
  \enum NbSceneManager::BOUNDING_BOX

  Only the bounding boxes of the components in the scene will be
  rendered.
*/

/*!
  \enum NbSceneManager::StereoMode

  This enum defines the various stereo mode rendering strategies
  supported by the NbSceneManager class.
*/

/*!
  \enum NbSceneManager::MONO

  MONO rendering is the traditional non-stereo rendering mode.
*/

/*!
  \enum NbSceneManager::RED_CYAN

  This rendering mode does composite rendering in the same buffer for
  users with glasses with one red and one cyan lens.  The red and the
  cyan counterparts of the model are only rendered as their respective
  color components of the model.  No monochromification of the model
  is performed, meaning each eye looses something.  This technique uses
  two rendering passes.
*/

/*!
  \enum NbSceneManager::RED_BLUE

  This rendering mode does composite rendering in the same buffer for
  users with glasses with one red and one blue lens.  The red and the
  blue counterparts of the model are only rendered as their respective
  color components of the model.  No monochromification of the model
  is performed, meaning each eye looses something, and both eyes loose
  the green component.  This technique uses two rendering passes.
*/

/*!
  \enum NbSceneManager::QUAD_BUFFER

  Quad buffer rendering renders full versions of the model into
  separate buffers for each eye.  Special hardware is used for
  displaying only the relevant buffer to the corresponding eye.
  Interleaved rendering on the same surface combined with shutter
  glasses, or polarized light rendering combined with polarizing
  filter glasses are examples of such hardware.  This technique uses
  two rendering passes.
*/

/*!
  \enum NbSceneManager::INTERLEAVED_ROWS

  This stereo mode will on ordinary displays render each other row for
  the left eye and the remaining rows for the right eye.  In
  combination with a video splitter, the image for each eye can be
  piped to the correct display.  Usually this is a set of glasses with
  video displays in them that you can connect to the graphics card.
  This technique uses two rendering passes.
*/

/*!
  \enum NbSceneManager::INTERLEAVED_COLUMNS

  This stereo mode will on ordinary displays render each other column for
  the left eye and the remaining columns for the right eye.  In
  combination with a video splitter, the image for each eye can be
  piped to the correct display.  Usually this is a set of glasses with
  video displays in them that you can connect to the graphics card.
  This technique uses two rendering passes.
*/

/*!
  \enum NbSceneManager::NavigationState

  This enum defines the available strategies for combining camera
  navigation and scene interaction when listening to SoEvent objects.
*/

/*!
  \enum NbSceneManager::NO_NAVIGATION

  Make all events go to the scene graph and not be passed to the
  current navigation system.
*/

/*!
  \enum NbSceneManager::JUST_NAVIGATION

  Make all events be passed to the current navigation system, and not
  be passed to the scene graph.
*/

/*!
  \enum NbSceneManager::MIXED_NAVIGATION

  Make all events be passed first to the scene graph.  If an
  interactive compomnent in the scene graph, such as a dragger or a
  selection node, uses the event, then the navigation system will be
  ignored.  If no component in the scene graph uses the event, then
  the navigation system is passed the event instead.
*/


/*!
  \enum NbSceneManager::AutoClippingStrategy

  This enum defines the autoclipping strategies that the
  NbSceneManager can handle.
*/

/*!
  \enum NbSceneManager::NO_AUTO_CLIPPING

  The SoCamera settings for controlling the near and far clipping
  planes will be left completely alone, and updating them correctly is
  left to the application developer.
*/

/*!
  \enum NbSceneManager::FIXED_NEAR_PLANE

  The near clipping plane will always be a fixed distance from the
  camera.  If you have problems with too little resolution of the
  depth buffer at far distances in the 3D model, increasing the near
  plane distance can often help, but with the tradeoff of cutting
  up-close geometry noticably far from the camera.
*/

/*!
  \enum NbSceneManager::VARIABLE_NEAR_PLANE

  The bounding box of the scene will be calculated to fit the near and
  far plane as good as possible around the scene to utilize the depth
  buffer as good as possible over the whole range of the model.  This
  technique can cause noticable extra overhead in special cases where
  scene graphs are built in ways that cause bounding box caching to be
  impossible to do.
*/

// *************************************************************************

// private implementation class

class NbSceneManagerP {
public:
  NbSceneManagerP(NbSceneManager * api);
  ~NbSceneManagerP(void);

  void clearBuffers(SbBool color, SbBool depth);
  SoCamera * getCamera(void);
  void touch(void);
  void renderSingle(SoGLRenderAction * action,
                    SbBool initmatrices,
                    SbBool clearwindow,
                    SbBool clearzbuffer);

public:
  NbSceneManager::RenderMode rendermode;
  NbSceneManager::StereoMode stereomode;

  SbBool texturesenabled;
  float stereooffset;
  SoInfo * dummynode;
  SoSearchAction searchaction;
  SoCamera * camera;
  SoColorPacker colorpacker;
  SbColor overlaycolor;

  NbSceneManager::NavigationState navigationstate;
  NbNavigationSystem * navigationsystem;

  SoGetBoundingBoxAction * getbboxaction;
  SoGetMatrixAction * getmatrixaction;

  int depthbits;
  NbSceneManager::AutoClippingStrategy autoclipping;
  float nearplanevalue;
  SoNodeSensor * autoclipsensor;
  static void update_clipping_planes(void * cosure, SoSensor * sensor);

  void setClippingPlanes(void);
  void getCameraCoordinateSystem(SbMatrix & matrix, SbMatrix & inverse);

protected:
  NbSceneManager * master;

}; // NbSceneManagerP

// *************************************************************************

#define PRIVATE(obj) obj->pimpl

/*!
  Constructor.
*/

NbSceneManager::NbSceneManager(void)
{
  PRIVATE(this) = new NbSceneManagerP(this);
  if (NbViewerNavigationMode::getClassTypeId() == SoType::badType()) {
    SoDebugError::post("NbSceneManager::NbSceneManager",
		       "SIM Nuts'n'Bolts library is not initialized. "
		       "See NutsnBolts::init().");
    exit(-1);
  }
  PRIVATE(this)->rendermode = AS_IS;
  PRIVATE(this)->stereomode = MONO;
  PRIVATE(this)->stereooffset = 0.1f;
  PRIVATE(this)->texturesenabled = TRUE;
  PRIVATE(this)->camera = NULL;
  PRIVATE(this)->overlaycolor = SbColor(1.0f, 0.0f, 0.0f);
  PRIVATE(this)->dummynode = new SoInfo;
  PRIVATE(this)->dummynode->ref();
  PRIVATE(this)->autoclipsensor =
    new SoNodeSensor(NbSceneManagerP::update_clipping_planes, PRIVATE(this));
  PRIVATE(this)->autoclipsensor->setPriority(this->getRedrawPriority() - 1);
  PRIVATE(this)->navigationsystem =
    NbNavigationSystem::getByName(NB_IDLER_SYSTEM);
}

/*!
  Destructor
*/

NbSceneManager::~NbSceneManager(void)
{
  this->setSceneGraph(NULL);
  PRIVATE(this)->dummynode->unref();
  if (PRIVATE(this)->autoclipsensor) {
    delete PRIVATE(this)->autoclipsensor;
    PRIVATE(this)->autoclipsensor = NULL;
  }
  if (PRIVATE(this)->camera) {
    PRIVATE(this)->camera->unref();
    PRIVATE(this)->camera = NULL;
  }
  if (PRIVATE(this)->getbboxaction) {
    delete PRIVATE(this)->getbboxaction;
    PRIVATE(this)->getbboxaction = NULL;
  }
  if (PRIVATE(this)->getmatrixaction) {
    delete PRIVATE(this)->getmatrixaction;
    PRIVATE(this)->getmatrixaction = NULL;
  }
  if (PRIVATE(this)->navigationsystem) {
    delete PRIVATE(this)->navigationsystem;
    PRIVATE(this)->navigationsystem = NULL;
  }
  delete PRIVATE(this);
}

// doc in superclass
void 
NbSceneManager::render(const SbBool clearwindow,
                       const SbBool clearzbuffer)
{
  if (PRIVATE(this)->depthbits < 0) {
    GLint depthbits[1];
    glGetIntegerv(GL_DEPTH_BITS, depthbits);
    PRIVATE(this)->depthbits = depthbits[0];
  }
  inherited::render(clearwindow, clearzbuffer);

  // FIXME: navigation mode rendering

#ifdef NB_EXTRA_DEBUG
  SoCamera * camera = PRIVATE(this)->getCamera();
  SoType utmcameratype = SoType::fromName("UTMCamera");
  if (camera != NULL && camera->isOfType(utmcameratype)) {
    SbVec3f campos = camera->position.getValue();
    if (campos != SbVec3f(0.0f, 0.0f, 0.0f)) {
      static int shown = FALSE;
      if (!shown) {
        SoDebugError::postWarning("NbSceneManger::render",
                                  "You are using an UTMCamera, and the "
                                  "non-utm camera position has strayed from "
                                  "origo (%g, %g, %g), meaning your navigation "
                                  "code is not UTM aware. This ought to be "
                                  "fixed. This warning will not be shown "
                                  "again.", campos[0], campos[1], campos[2]);
        shown = TRUE;
      }
#if NB_EXTRA_DEBUG > 1
      exit(-1);
#endif // NB_EXTRA_DEBUG > 1
    }
  }
#endif // NB_EXTRA_DEBUG
}

// doc in superclass
void 
NbSceneManager::render(SoGLRenderAction * action,
                       const SbBool initmatrices,
                       const SbBool clearwindow,
                       const SbBool clearzbuffer)
{
  if (PRIVATE(this)->depthbits < 0) {
    GLint depthbits[1];
    glGetIntegerv(GL_DEPTH_BITS, depthbits);
    PRIVATE(this)->depthbits = depthbits[0];
  }

  if (PRIVATE(this)->stereomode == MONO) {
    PRIVATE(this)->renderSingle(action, initmatrices, clearwindow, clearzbuffer);
  } else {
    SoCamera * camera = PRIVATE(this)->getCamera();
    if (!camera) return;

    PRIVATE(this)->clearBuffers(TRUE, TRUE);
    camera->setStereoAdjustment(PRIVATE(this)->stereooffset);
    camera->setStereoMode(SoCamera::LEFT_VIEW);
    
    switch (PRIVATE(this)->stereomode) {      
    case RED_CYAN:
    case RED_BLUE:
      glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
      break;
    case QUAD_BUFFER:
    case INTERLEAVED_ROWS:
    case INTERLEAVED_COLUMNS:
      assert(0 && "not implemented yet");
      break;
    default:
      assert(0 && "unknown stereo mode");
      break;
    }
    PRIVATE(this)->renderSingle(action, initmatrices, FALSE, FALSE);

    camera->setStereoMode(SoCamera::RIGHT_VIEW);
    switch (PRIVATE(this)->stereomode) {      
    case RED_CYAN:      
      glClear(GL_DEPTH_BUFFER_BIT);
      glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
      break;
    case RED_BLUE:
      glClear(GL_DEPTH_BUFFER_BIT);
      glColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
      break;
    case QUAD_BUFFER:
    case INTERLEAVED_ROWS:
    case INTERLEAVED_COLUMNS:
      assert(0 && "not implemented yet");
      break;
    default:
      assert(0 && "unknown stereo mode");
      break;
    }
    PRIVATE(this)->renderSingle(action, initmatrices, FALSE, FALSE);

    // restore
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    camera->setStereoMode(SoCamera::MONOSCOPIC);
  }
  // FIXME: navigation mode rendering
}

/*!  
  Sets the camera to be used.  If you do not set a camera, the scene manager
  will search for one by itself.

  \sa getCamera
*/

void 
NbSceneManager::setCamera(SoCamera * camera)
{
  if (PRIVATE(this)->camera) {
    PRIVATE(this)->camera->unref();
  }
  PRIVATE(this)->camera = camera;
  if (camera) camera->ref();
  assert(PRIVATE(this)->navigationsystem);
  PRIVATE(this)->navigationsystem->setCamera(this->getCamera());
}

/*!
  Returns the current camera.

  \sa setCamera
*/

SoCamera * 
NbSceneManager::getCamera(void) const
{
  return PRIVATE(this)->getCamera();
}

/*!
  Sets the render mode. Defaults to AS_IS.

  \sa NbSceneManager::RenderMode, getRenderMode
*/

void 
NbSceneManager::setRenderMode(const RenderMode mode)
{
  PRIVATE(this)->rendermode = mode;
}

/*!
  Returns the current render mode.

  \sa NbSceneManager::RenderMode, setRenderMode
*/

NbSceneManager::RenderMode 
NbSceneManager::getRenderMode(void) const
{
  return PRIVATE(this)->rendermode;
}

/*!
  Sets the stereo rendering mode. Defaults to MONO.

  \sa getStereoMode
*/

void 
NbSceneManager::setStereoMode(const StereoMode mode)
{
  PRIVATE(this)->stereomode = mode;
  PRIVATE(this)->touch();
}

/*!
  Returns the current stereo mode.

  \sa setStereoMode
*/

NbSceneManager::StereoMode 
NbSceneManager::getStereoMode(void) const
{
  return PRIVATE(this)->stereomode;
}

/*!
  Sets the stereo offset - the distance between the viewpoint each
  eye uses - used when doing stereo rendering.
  Defaults to 0.1.

  \sa getStereoOffset
*/

void 
NbSceneManager::setStereoOffset(const float offset)
{
  PRIVATE(this)->stereooffset = offset;
  PRIVATE(this)->touch();
}

/*!
  Returns the current stereo offset.

  \sa setStereoOffset
*/

float 
NbSceneManager::getStereoOffset(void) const
{
  return PRIVATE(this)->stereooffset;
}

/*!
  Enable/disable textures when rendering.
  Defaults to TRUE.

  \sa isTexturesEnabled
*/

void 
NbSceneManager::setTexturesEnabled(const SbBool onoff)
{
  PRIVATE(this)->texturesenabled = onoff;
  PRIVATE(this)->touch();
}

/*!
  Returns whether textures are enabled or not.

  \sa setTexturesEnabled
*/

SbBool 
NbSceneManager::isTexturesEnabled(void) const
{
  return PRIVATE(this)->texturesenabled;
}

/*!
  Sets the color of the lines in WIREFRAME_OVERLAY rendering mode.
  Defaults to red [1.0 0.0 0.0].

  \sa getWireframeOverlayColor
*/

void 
NbSceneManager::setWireframeOverlayColor(const SbColor & color)
{
  PRIVATE(this)->overlaycolor = color;
}

/*!
  Returns the WIREFRAME_OVERLAY line color.

  \sa setWireframeOverlayColor
*/

const SbColor & 
NbSceneManager::getWireframeOverlayColor(void) const
{
  return PRIVATE(this)->overlaycolor;
}

/*!
  Overloaded from SoSceneManager to handle viewer navigation.
*/

SbBool
NbSceneManager::processEvent(const SoEvent * const event)
{
  // fprintf(stderr, "NbSceneManager::processEvent()\n");
  const SbViewportRegion & vp = this->getViewportRegion();
  assert(PRIVATE(this)->navigationsystem);
  PRIVATE(this)->navigationsystem->setViewport(vp);
  // PRIVATE(this)->navigationsystem->setSceneGraph(this->getSceneGraph());

  switch (PRIVATE(this)->navigationstate) {
  case NbSceneManager::NO_NAVIGATION:
    if (inherited::processEvent(event))
      return TRUE;
    break;
  case NbSceneManager::JUST_NAVIGATION:
    if (PRIVATE(this)->navigationsystem->processEvent(event))
      return TRUE;
    break;
  case NbSceneManager::MIXED_NAVIGATION:
    // see if dragger is used first, then do navigation if not
    if (inherited::processEvent(event))
      return TRUE;
    if (PRIVATE(this)->navigationsystem->processEvent(event))
      return TRUE;
    break;
  }
  return FALSE;
}

/*!
  This method sets the navigation mode of the scene manager.  The navigation
  mode decides the system the user uses to navigate the 3D model.

  NbSceneManager::NO_NAVIGATION is the default setting.

  \sa NbSceneManager::NavigationState, getNavigationState
*/

void
NbSceneManager::setNavigationState(NavigationState state)
{
  PRIVATE(this)->navigationstate = state;
}

/*!
  This method returns which state the camera navigation system is in.

  \sa NbSceneManager::NavigationState, setNavigationState
*/
NbSceneManager::NavigationState
NbSceneManager::getNavigationState(void) const
{
  return PRIVATE(this)->navigationstate;
}

/*!
  This method sets the navigation system to use.

  \sa NbNavigationSystem, getNavigationSystem
*/

void
NbSceneManager::setNavigationSystem(NbNavigationSystem * system)
{
  if (system) {
    PRIVATE(this)->navigationsystem = system;
  } else {
    PRIVATE(this)->navigationsystem =
      NbNavigationSystem::getByName(NB_IDLER_SYSTEM);
  }
  PRIVATE(this)->navigationsystem->setSceneGraph(this->getSceneGraph());
  PRIVATE(this)->navigationsystem->setCamera(this->getCamera());
  PRIVATE(this)->navigationsystem->setViewport(this->getViewportRegion());
  PRIVATE(this)->navigationsystem->invokeModeChangeCallbacks();
}

/*!
  This method returns the current navigation system in use.
  NULL means that no navigation system is in use.

  \sa NbNavigationSystem, setNavigationSystem
*/

NbNavigationSystem *
NbSceneManager::getNavigationSystem(void) const
{
  return PRIVATE(this)->navigationsystem;
}

/*!
  Overloaded in case of future developments.

  See SoSceneManager::setSceneGraph().
*/

void
NbSceneManager::setSceneGraph(SoNode * const root)
{
  // re-connect autoclipping update sensor
  if (PRIVATE(this)->autoclipsensor->isScheduled()) {
    PRIVATE(this)->autoclipsensor->unschedule();
  }
  if (PRIVATE(this)->autoclipsensor->getAttachedNode()) {
    PRIVATE(this)->autoclipsensor->detach();
  }

  inherited::setSceneGraph(root);

  if (root) {
    PRIVATE(this)->autoclipsensor->attach(root);
    if (PRIVATE(this)->autoclipping != NbSceneManager::NO_AUTO_CLIPPING) {
      PRIVATE(this)->setClippingPlanes();
    }

    // set up navigation mode if scene graph contains navigation mode node.
    PRIVATE(this)->searchaction.reset();
    PRIVATE(this)->searchaction.setType(SoCamera::getClassTypeId());
    PRIVATE(this)->searchaction.setInterest(SoSearchAction::FIRST);
    PRIVATE(this)->searchaction.apply(root);
    SoPath * path = PRIVATE(this)->searchaction.getPath();
    if (path) {
      path->ref();
      PRIVATE(this)->searchaction.reset();
      PRIVATE(this)->searchaction.setType(NbViewerNavigationMode::getClassTypeId());
      PRIVATE(this)->searchaction.setInterest(SoSearchAction::FIRST);
      PRIVATE(this)->searchaction.apply(path);
      path->unref();
      path = PRIVATE(this)->searchaction.getPath();
      if (path) {
        path->ref();
        NbViewerNavigationMode * mode = (NbViewerNavigationMode *) path->getTail();
        assert(mode && mode->isOfType(NbViewerNavigationMode::getClassTypeId()));
        SbString modestring = mode->mode.getValue();
        path->unref();
        if (modestring.getLength() > 0) {
          NbNavigationSystem * system =
            NbNavigationSystem::getByName(modestring.getString());
          this->setNavigationSystem(system);
        } else {
          // node but no string - use a default system
          NbNavigationSystem * system =
            NbNavigationSystem::getByName(NB_DEFAULT_SYSTEM);
          this->setNavigationSystem(system);
        }
      }
    }
    PRIVATE(this)->searchaction.reset();
  }

  assert(PRIVATE(this)->navigationsystem);
  PRIVATE(this)->navigationsystem->setSceneGraph(root);
  PRIVATE(this)->navigationsystem->setCamera(this->getCamera());
}

/*!
  Overloaded in case of future developments.

  See SoSceneManager::getSceneGraph().
*/

SoNode *
NbSceneManager::getSceneGraph(void) const
{
  return inherited::getSceneGraph();
}

/*!
  Overloaded in case of future developments.

  See SoSceneManager::activate().
*/

void
NbSceneManager::activate(void)
{
  inherited::activate();
}

/*!
  Overloaded in case of future developments.

  See SoSceneManager::deactivate().
*/

void
NbSceneManager::deactivate(void)
{
  inherited::deactivate();
}

/*!
  This method sets the scene manager's autoclipping strategy.

  Default setting is NbSceneManager::VARIABLE_NEAR_PLANE.

  \sa NbSceneManager::AutoClippingStrategy, getAutoClipping, setNearPlaneValue, getNearPlaneValue
*/

void
NbSceneManager::setAutoClipping(AutoClippingStrategy strategy)
{
  PRIVATE(this)->autoclipping = strategy;
  switch (strategy) {
  case NO_AUTO_CLIPPING:
    if (PRIVATE(this)->autoclipsensor->isScheduled()) {
      PRIVATE(this)->autoclipsensor->unschedule();
    }
    if (PRIVATE(this)->autoclipsensor->getAttachedNode()) {
      PRIVATE(this)->autoclipsensor->detach();
    }
    break;
  case FIXED_NEAR_PLANE:
  case VARIABLE_NEAR_PLANE:
    if (!PRIVATE(this)->autoclipsensor->getAttachedNode()) {
      PRIVATE(this)->autoclipsensor->attach(this->getSceneGraph());
    }
    PRIVATE(this)->autoclipsensor->schedule();
    break;
  }
}

/*!
  This method returns the current autoclipping strategy.

  \sa setAutoClipping
*/

NbSceneManager::AutoClippingStrategy
NbSceneManager::getAutoClipping(void) const
{
  return PRIVATE(this)->autoclipping;
}

/*!
  When the NbSceneManager::FIXED_NEAR_PLANE autoclipping strategy is
  used, you set the value of the near plane distance with this method.

  \sa setAutoClipping, getNearPlaneValue, NbSceneManager::AutoClippingStrategy
*/

void
NbSceneManager::setNearPlaneValue(float value)
{
  PRIVATE(this)->nearplanevalue = value;
}

/*!
  This method returns the near plane distance value that will be used
  when the NbSceneManager::FIXED_NEAR_PLANE autoclipping strategy is used.

  Default value is 0.6.

  \sa setAutoClipping, setNearPlaneValue,  NbSceneManager::AutoClippingStrategy
*/

float
NbSceneManager::getNearPlaneValue(void) const
{
  return PRIVATE(this)->nearplanevalue;
}

#undef PRIVATE

// *************************************************************************
// private implementation class

NbSceneManagerP::NbSceneManagerP(NbSceneManager * api)
{
  this->master = api;
  this->navigationstate = NbSceneManager::NO_NAVIGATION;
  this->navigationsystem = NULL;
  this->autoclipping = NbSceneManager::VARIABLE_NEAR_PLANE;
  this->nearplanevalue = 0.6f;
  this->getbboxaction = NULL;
  this->getmatrixaction = NULL;
  this->autoclipsensor = NULL;
  this->depthbits = -1;
}

NbSceneManagerP::~NbSceneManagerP(void)
{
}

void
NbSceneManagerP::clearBuffers(SbBool color, SbBool depth)
{
  const SbColor bgcol = this->master->getBackgroundColor();
  GLbitfield mask = 0;
  if (color) mask |= GL_COLOR_BUFFER_BIT;
  if (depth) mask |= GL_DEPTH_BUFFER_BIT;
  glClearColor(bgcol[0], bgcol[1], bgcol[2], 0.0f);
  glClear(mask);
}

SoCamera *
NbSceneManagerP::getCamera(void)
{
  if (this->camera) return this->camera;
  if (!master->getSceneGraph()) return NULL;
  this->searchaction.setType(SoCamera::getClassTypeId());
  this->searchaction.setInterest(SoSearchAction::FIRST);
  SbBool old = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  this->searchaction.apply(master->getSceneGraph());
  SoBaseKit::setSearchingChildren(old);
  SoFullPath * path = (SoFullPath *) this->searchaction.getPath();
  if (path) {
    SoNode * tail = path->getTail();
    this->searchaction.reset();
    return (SoCamera *) tail;
  }
  return NULL;
}

// touch internal node (used when setting element values)
void 
NbSceneManagerP::touch(void)
{
  this->dummynode->touch();
  // do not trigger a redraw here. User might set the render mode
  // right before calling render().
}

// render once in correct draw style
void 
NbSceneManagerP::renderSingle(SoGLRenderAction * action,
			      SbBool initmatrices,
			      SbBool clearwindow,
			      SbBool clearzbuffer)
{
  SoState * state = action->getState();
  state->push();

  SoNode * node = this->dummynode;

  if (!this->texturesenabled) {
    SoTextureQualityElement::set(state, node, 0.0f);
    SoTextureOverrideElement::setQualityOverride(state, TRUE);
  }
  
  switch (this->rendermode) {
  case NbSceneManager::AS_IS:
    master->SoSceneManager::render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case NbSceneManager::WIREFRAME:
    SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
    SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
    SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
    SoOverrideElement::setLightModelOverride(state, node, TRUE);
    master->SoSceneManager::render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case NbSceneManager::POINTS:
    SoDrawStyleElement::set(state, node, SoDrawStyleElement::POINTS);
    SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
    SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
    SoOverrideElement::setLightModelOverride(state, node, TRUE);
    master->SoSceneManager::render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  case NbSceneManager::HIDDEN_LINE:
    {
      // must clear before setting draw mask
      this->clearBuffers(TRUE, TRUE);

      // only draw into depth buffer
      glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
      SoMaterialBindingElement::set(state, node, SoMaterialBindingElement::OVERALL);
      SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
      SoPolygonOffsetElement::set(state, node, 1.0f, 1.0f,
                                  SoPolygonOffsetElement::FILLED, TRUE);
      SoOverrideElement::setPolygonOffsetOverride(state, node, TRUE);
      SoOverrideElement::setLightModelOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, TRUE);
      master->SoSceneManager::render(action, initmatrices, FALSE, FALSE);

      // reenable draw masks
      glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
      SoPolygonOffsetElement::set(state, node, 0.0f, 0.0f,
                                  SoPolygonOffsetElement::FILLED, FALSE);
      SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
      SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, FALSE);
      master->SoSceneManager::render(action, initmatrices, FALSE, FALSE);    
    }
    break;
  case NbSceneManager::WIREFRAME_OVERLAY:
      SoPolygonOffsetElement::set(state, node, 1.0f, 1.0f,
                                  SoPolygonOffsetElement::FILLED, TRUE);
      SoOverrideElement::setPolygonOffsetOverride(state, node, TRUE);
      master->SoSceneManager::render(action, initmatrices, clearwindow, clearzbuffer);
      SoPolygonOffsetElement::set(state, node, 0.0f, 0.0f,
                                  SoPolygonOffsetElement::FILLED, FALSE);
      
      SoLazyElement::setDiffuse(state, node, 1, &this->overlaycolor, 
                                &this->colorpacker);
      SoLightModelElement::set(state, node, SoLightModelElement::BASE_COLOR);
      SoMaterialBindingElement::set(state, node, SoMaterialBindingElement::OVERALL);
      SoDrawStyleElement::set(state, node, SoDrawStyleElement::LINES);
      SoOverrideElement::setLightModelOverride(state, node, TRUE);
      SoOverrideElement::setDiffuseColorOverride(state, node, TRUE);
      SoOverrideElement::setMaterialBindingOverride(state, node, TRUE);
      SoOverrideElement::setDrawStyleOverride(state, node, TRUE);
      master->SoSceneManager::render(action, initmatrices, FALSE, FALSE);    
    break;

  case NbSceneManager::BOUNDING_BOX:
    SoComplexityTypeElement::set(state, node, SoComplexityTypeElement::BOUNDING_BOX);
    SoOverrideElement::setComplexityTypeOverride(state, node, TRUE);
    master->SoSceneManager::render(action, initmatrices, clearwindow, clearzbuffer);
    break;
  default:
    assert(0 && "unknown rendering mode");
    break;
  }
  state->pop();
}

void
NbSceneManagerP::setClippingPlanes(void)
{
  SoCamera * camera = this->getCamera();
  if (!camera) { return; }
  SoNode * root = this->master->getSceneGraph();
  if (!root) { return; }

  SbViewportRegion vp = this->master->getViewportRegion();

  if (!this->getbboxaction) {
    this->getbboxaction =
      new SoGetBoundingBoxAction(this->master->getViewportRegion());
  } else {
    this->getbboxaction->setViewportRegion(this->master->getViewportRegion());
  }
  this->getbboxaction->apply(root);

  SbXfBox3f xbox = this->getbboxaction->getXfBoundingBox();
  SbMatrix cammat;
  SbMatrix inverse;
  this->getCameraCoordinateSystem(cammat, inverse);
  xbox.transform(inverse);

  SbMatrix mat;
  mat.setTranslate(- camera->position.getValue());
  xbox.transform(mat);
  mat = camera->orientation.getValue().inverse();
  xbox.transform(mat);
  SbBox3f box = xbox.project();

  float nearval = -box.getMax()[2];
  float farval = -box.getMin()[2];

  if (farval <= 0.0f) return;

  if (camera->isOfType(SoPerspectiveCamera::getClassTypeId())) {
    float nearlimit;
    if (this->autoclipping == NbSceneManager::FIXED_NEAR_PLANE) {
      nearlimit = this->nearplanevalue;
    } else {
      int depthbits = this->depthbits;
      if (depthbits < 0) depthbits = 32;
      int use_bits = (int) (float(depthbits) * (1.0f - this->nearplanevalue));
      float r = (float) pow(2.0, double(use_bits));
      nearlimit = farval / r;
    }

    if (nearlimit >= farval) {
      nearlimit = farval / 5000.0f;
    }

    if (nearval < nearlimit) {
      nearval = nearlimit;
    }
  }

  const float SLACK = 0.001f;

  SbBool oldnear = camera->nearDistance.enableNotify(FALSE);
  SbBool oldfar = camera->farDistance.enableNotify(FALSE);

  camera->nearDistance = nearval * (1.0f - SLACK);
  camera->farDistance = farval * (1.0f + SLACK);

  if (oldnear) {
    camera->nearDistance.enableNotify(TRUE);
  }
  if (oldfar) {
    camera->farDistance.enableNotify(TRUE);
  }

}

void
NbSceneManagerP::getCameraCoordinateSystem(SbMatrix & matrix, SbMatrix & inverse)
{
  matrix = inverse = SbMatrix::identity();

  SoCamera * camera = this->getCamera();
  SoNode * root = this->master->getSceneGraph();
  assert(camera && root);

  this->searchaction.reset();
  this->searchaction.setSearchingAll(TRUE);
  this->searchaction.setInterest(SoSearchAction::FIRST);
  this->searchaction.setNode(camera);
  this->searchaction.apply(root);

  if (this->searchaction.getPath()) {
    if (!this->getmatrixaction) {
      this->getmatrixaction =
	new SoGetMatrixAction(this->master->getViewportRegion());
    } else {
      this->getmatrixaction->setViewportRegion(this->master->getViewportRegion());
    }
    this->getmatrixaction->apply(this->searchaction.getPath());
    matrix = this->getmatrixaction->getMatrix();
    inverse = this->getmatrixaction->getInverse();
  }
  this->searchaction.reset();
}

void
NbSceneManagerP::update_clipping_planes(void * closure, SoSensor * sensor)
{
  NbSceneManagerP * mgrp = (NbSceneManagerP *) closure;
  if (mgrp->autoclipping != NbSceneManager::NO_AUTO_CLIPPING) {
    mgrp->setClippingPlanes();
  }
}

// *************************************************************************
