#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <stdio.h>

#include <Inventor/SbBox.h>
#include <Inventor/SbXfBox3f.h>
#include <Inventor/SbMatrix.h>
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

/*!
  \class NbSceneManager NutsnBolts/NbSceneManager.h
  \brief Extends the SoSceneManager class with viewer-related functionality.

  The NbSceneManager class extends SoSceneManger with functionality that
  relates to scene graphs and rendering on a higher level.

  This class makes it possible to easily control the draw style of a
  scene. It also has support for controlling stereo rendering.  In addition,
  it handles user events to do viewer navigation.

  \ingroup misc
  \ingroup navigation
*/

// *************************************************************************

class NbSceneManagerP {
public:
  NbSceneManagerP(NbSceneManager * api);

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

};

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
  if ( this->camera ) return this->camera;
  this->searchaction.setType(SoCamera::getClassTypeId());
  this->searchaction.setInterest(SoSearchAction::FIRST);
  SbBool old = SoBaseKit::isSearchingChildren();
  SoBaseKit::setSearchingChildren(TRUE);
  this->searchaction.apply(master->getSceneGraph());
  SoBaseKit::setSearchingChildren(old);
  SoFullPath * path = (SoFullPath *) this->searchaction.getPath();
  if ( path ) {
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
  
  switch ( this->rendermode ) {
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
  if ( !camera ) {
    return;
  }
  SoNode * root = this->master->getSceneGraph();
  if ( !root ) {
    return;
  }

  SbViewportRegion vp = this->master->getViewportRegion();

  if ( !this->getbboxaction ) {
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

  if ( farval <= 0.0f ) return;

  if ( camera->isOfType(SoPerspectiveCamera::getClassTypeId()) ) {
    float nearlimit;
    if ( this->autoclipping == NbSceneManager::FIXED_NEAR_PLANE ) {
      nearlimit = this->nearplanevalue;
    } else {
      int depthbits = this->depthbits;
      if ( depthbits < 0 ) depthbits = 32;
      int use_bits = (int) (float(depthbits) * (1.0f - this->nearplanevalue));
      float r = (float) pow(2.0, double(use_bits));
      nearlimit = farval / r;
    }

    if ( nearlimit >= farval ) {
      nearlimit = farval / 5000.0f;
    }

    if ( nearval < nearlimit ) {
      nearval = nearlimit;
    }
  }

  const float SLACK = 0.001f;

  SbBool oldnear = camera->nearDistance.enableNotify(FALSE);
  SbBool oldfar = camera->farDistance.enableNotify(FALSE);

  camera->nearDistance = nearval * (1.0f - SLACK);
  camera->farDistance = farval * (1.0f - SLACK);

  if ( oldnear ) {
    camera->nearDistance.enableNotify(TRUE);
  }
  if ( oldfar ) {
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

  if ( this->searchaction.getPath() ) {
    if ( !this->getmatrixaction ) {
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
  if ( mgrp->autoclipping != NbSceneManager::NO_AUTO_CLIPPING ) {
    mgrp->setClippingPlanes();
  }
}

// *************************************************************************

#define PRIVATE(obj) obj->pimpl

/*!
  Constructor.
*/

NbSceneManager::NbSceneManager(void)
{
  PRIVATE(this) = new NbSceneManagerP(this);
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
}

/*!
  Destructor
*/

NbSceneManager::~NbSceneManager(void)
{
  PRIVATE(this)->dummynode->unref();
  if ( PRIVATE(this)->autoclipsensor ) {
    delete PRIVATE(this)->autoclipsensor;
    PRIVATE(this)->autoclipsensor = NULL;
  }
  if ( PRIVATE(this)->camera ) {
    PRIVATE(this)->camera->unref();
    PRIVATE(this)->camera = NULL;
  }
  if ( PRIVATE(this)->getbboxaction ) {
    delete PRIVATE(this)->getbboxaction;
    PRIVATE(this)->getbboxaction = NULL;
  }
  if ( PRIVATE(this)->getmatrixaction ) {
    delete PRIVATE(this)->getmatrixaction;
    PRIVATE(this)->getmatrixaction = NULL;
  }
  delete PRIVATE(this);
}

// doc in superclass
void 
NbSceneManager::render(const SbBool clearwindow,
                       const SbBool clearzbuffer)
{
  if ( PRIVATE(this)->depthbits < 0 ) {
    GLint depthbits[1];
    glGetIntegerv(GL_DEPTH_BITS, depthbits);
    PRIVATE(this)->depthbits = depthbits[0];
  }
  inherited::render(clearwindow, clearzbuffer);
  // FIXME: navigation mode rendering
}

// doc in superclass
void 
NbSceneManager::render(SoGLRenderAction * action,
                       const SbBool initmatrices,
                       const SbBool clearwindow,
                       const SbBool clearzbuffer)
{
  if ( PRIVATE(this)->depthbits < 0 ) {
    GLint depthbits[1];
    glGetIntegerv(GL_DEPTH_BITS, depthbits);
    PRIVATE(this)->depthbits = depthbits[0];
  }

  if ( PRIVATE(this)->stereomode == MONO ) {
    PRIVATE(this)->renderSingle(action, initmatrices, clearwindow, clearzbuffer);
  } else {
    SoCamera * camera = PRIVATE(this)->getCamera();
    if ( !camera ) return;

    PRIVATE(this)->clearBuffers(TRUE, TRUE);
    camera->setStereoAdjustment(PRIVATE(this)->stereooffset);
    camera->setStereoMode(SoCamera::LEFT_VIEW);
    
    switch ( PRIVATE(this)->stereomode ) {      
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
    switch ( PRIVATE(this)->stereomode ) {      
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
  Sets the camera to be used. If you do not set a camera, the
  manager will search the scene graph for a camera (every frame).
  Set the camera here to avoid this search.
*/

void 
NbSceneManager::setCamera(SoCamera * camera)
{
  if ( PRIVATE(this)->camera ) {
    PRIVATE(this)->camera->unref();
  }
  PRIVATE(this)->camera = camera;
  if ( camera ) camera->ref();
  if ( PRIVATE(this)->navigationsystem )
    PRIVATE(this)->navigationsystem->setCamera(this->getCamera());
}

/*!
  Returns the current camera. If no camera has been set, the current
  scene graph will be searched, and the first active camera will be
  returned.
*/

SoCamera * 
NbSceneManager::getCamera(void) const
{
  return PRIVATE(this)->getCamera();
}

/*!
  Sets the render mode.
*/

void 
NbSceneManager::setRenderMode(const RenderMode mode)
{
  PRIVATE(this)->rendermode = mode;
}

/*!
  Returns the current render mode.
*/

NbSceneManager::RenderMode 
NbSceneManager::getRenderMode(void) const
{
  return PRIVATE(this)->rendermode;
}

/*!
  Sets the stereo mode.
*/

void 
NbSceneManager::setStereoMode(const StereoMode mode)
{
  PRIVATE(this)->stereomode = mode;
  PRIVATE(this)->touch();
}

/*!
  Returns the current stereo mode.
*/

NbSceneManager::StereoMode 
NbSceneManager::getStereoMode(void) const
{
  return PRIVATE(this)->stereomode;
}

/*!
  Sets the stereo offset used when doing stereo rendering.
*/

void 
NbSceneManager::setStereoOffset(const float offset)
{
  PRIVATE(this)->stereooffset = offset;
  PRIVATE(this)->touch();
}

/*!
  Returns the current stereo offset.
*/

float 
NbSceneManager::getStereoOffset(void) const
{
  return PRIVATE(this)->stereooffset;
}

/*!
  Enable/disable textures when rendering.
*/

void 
NbSceneManager::setTexturesEnabled(const SbBool onoff)
{
  PRIVATE(this)->texturesenabled = onoff;
  PRIVATE(this)->touch();
}

/*!
  Returns whether textures are enabled or not.
*/

SbBool 
NbSceneManager::isTexturesEnabled(void) const
{
  return PRIVATE(this)->texturesenabled;
}

/*!
  Sets the color of the lines in WIREFRAME_OVERLAY rendering mode.
*/

void 
NbSceneManager::setWireframeOverlayColor(const SbColor & color)
{
  PRIVATE(this)->overlaycolor = color;
}

/*!
  Returns the WIREFRAME_OVERLAY line color.
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
  PRIVATE(this)->navigationsystem->setViewport(vp);

  switch ( PRIVATE(this)->navigationstate ) {
  case NbSceneManager::NO_NAVIGATION:
    if ( inherited::processEvent(event) )
      return TRUE;
    break;
  case NbSceneManager::JUST_NAVIGATION:
    if ( PRIVATE(this)->navigationsystem != NULL &&
	 PRIVATE(this)->navigationsystem->processEvent(event) )
      return TRUE;
    break;
  case NbSceneManager::MIXED_NAVIGATION:
    // see if dragger is used first, then do navigation if not
    if ( inherited::processEvent(event) )
      return TRUE;
    if ( PRIVATE(this)->navigationsystem != NULL &&
	 PRIVATE(this)->navigationsystem->processEvent(event) )
      return TRUE;
    break;
  }
  return FALSE;
}

/*!
  This method sets the navigation mode of the scene manager.  The navigation
  mode decides the system the user uses to navigate the 3D model.

  Using NO_NAVIGATION, there is no camera navigastion happening, just
  scene interaction (draggers, manipulators, callbacks, selection,
  etc).

  Using JUST_NAVIGATION, the scene graph will not receive events so
  all i interactive scene components stay inactive and you just move
  the camera.

  Using MIXED_NAVIGATION, interactive components in the scene graph are
  checked first, and if none of them responds to the event, the event is
  used for navigating the camera instead.

  NO_NAVIGATION is the default setting.

  \sa getNavigationState
*/

void
NbSceneManager::setNavigationState(NavigationState state)
{
  PRIVATE(this)->navigationstate = state;
}

/*!
  This method returns which state the camera navigation system is in.

  \sa setNavigationEnabled
*/
NbSceneManager::NavigationState
NbSceneManager::getNavigationState(void) const
{
  return PRIVATE(this)->navigationstate;
}

void
NbSceneManager::setNavigationSystem(NbNavigationSystem * system)
{
  PRIVATE(this)->navigationsystem = system;
  PRIVATE(this)->navigationsystem->setCamera(this->getCamera());

}

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
  if ( PRIVATE(this)->autoclipsensor->isScheduled() ) {
    PRIVATE(this)->autoclipsensor->unschedule();
  }
  if ( root ) {
    PRIVATE(this)->autoclipsensor->attach(root);
    PRIVATE(this)->autoclipsensor->schedule();
  }

  inherited::setSceneGraph(root);

  // set up navigation mode if scene graph contains navigation mode node.
  PRIVATE(this)->searchaction.reset();
  PRIVATE(this)->searchaction.setType(SoCamera::getClassTypeId());
  PRIVATE(this)->searchaction.setInterest(SoSearchAction::FIRST);
  PRIVATE(this)->searchaction.apply(root);
  SoPath * path = PRIVATE(this)->searchaction.getPath();
  if ( path ) {
    path->ref();
    PRIVATE(this)->searchaction.reset();
    PRIVATE(this)->searchaction.setType(NbViewerNavigationMode::getClassTypeId());
    PRIVATE(this)->searchaction.setInterest(SoSearchAction::FIRST);
    PRIVATE(this)->searchaction.apply(path);
    path->unref();
    path = PRIVATE(this)->searchaction.getPath();
    if ( path ) {
      path->ref();
      NbViewerNavigationMode * mode = (NbViewerNavigationMode *) path->getTail();
      assert(mode && mode->isOfType(NbViewerNavigationMode::getClassTypeId()));
      SbString modestring = mode->mode.getValue();
      path->unref();
      if ( modestring.getLength() > 0 ) {
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

  if ( PRIVATE(this)->navigationsystem ) {
    PRIVATE(this)->navigationsystem->setCamera(this->getCamera());
  }
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

  In the case of NO_AUTO_CLIPPING, the camera in the scene graph will
  not be touched.

  In the case of VARIABLE_NEAR_PLANE, the near and far plane will be
  set to a close fit around the bounding box, to give the best
  possible depth buffer resolution.  The scene bounding box has to be
  recalculated whenever the scene graph changes for this setting,
  which can for certain special cases reduce overall performance.

  In the case of FIXED_NEAR_PLANE, the near plane will stay fixed at
  the same value all the time.  No bounding box has to be calculated
  in this case.

  Default setting is VARIABLE_NEAR_PLANE.

  \sa getAutoClipping(), setNearPlaneValue(), getNearPlaneValue()
*/

void
NbSceneManager::setAutoClipping(AutoClippingStrategy strategy)
{
  PRIVATE(this)->autoclipping = strategy;
}

/*!
  This method returns the current autoclipping strategy.

  \sa setAutoClipping().
*/

NbSceneManager::AutoClippingStrategy
NbSceneManager::getAutoClipping(void) const
{
  return PRIVATE(this)->autoclipping;
}

/*!
  When the FIXED_NEAR_PLANE autoclipping strategy is used, you set the
  value of the near plane distance with this method.

  \sa setAutoClipping(), getNearPlaneValue()
*/

void
NbSceneManager::setNearPlaneValue(float value)
{
  PRIVATE(this)->nearplanevalue = value;
}

/*!
  This method returns the near plane distance value that will be used
  when the FIXED_NEAR_PLANE autoclipping strategy is used.

  Default value is 0.6.

  \sa setAutoClipping(), setNearPlaneValue()
*/

float
NbSceneManager::getNearPlaneValue(void) const
{
  return PRIVATE(this)->nearplanevalue;
}

#undef PRIVATE
