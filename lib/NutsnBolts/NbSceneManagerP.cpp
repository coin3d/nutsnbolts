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

#include <NutsnBolts/NbSceneManager.h>
#include "NbSceneManagerP.h"

#include <Inventor/system/gl.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoComplexityTypeElement.h>
#include <Inventor/elements/SoPolygonOffsetElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoTextureOverrideElement.h>
#include <Inventor/elements/SoTextureQualityElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodes/SoInfo.h>

#include <NutsnBolts/nodes/NbViewerNavigationMode.h>
#include <NutsnBolts/navigation/NbNavigationSystem.h>

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

// defsys is the default system returned if none is found in root

NbNavigationSystem *
NbSceneManagerP::searchForNavigationMode(SoNode * root, 
                                         NbNavigationSystem * defsys)
{
  this->searchaction.reset();
  this->searchaction.setType(SoCamera::getClassTypeId());
  this->searchaction.setInterest(SoSearchAction::FIRST);
  this->searchaction.apply(root);
  SoPath * path = this->searchaction.getPath();
  
  if (!path) return defsys;
  
  path->ref();
  this->searchaction.reset();
  this->searchaction.setType(NbViewerNavigationMode::getClassTypeId());
  this->searchaction.setInterest(SoSearchAction::FIRST);
  this->searchaction.apply(path);
  path->unref();
  path = this->searchaction.getPath();

  if (!path) return defsys;

  path->ref();
  NbViewerNavigationMode * mode = (NbViewerNavigationMode *) path->getTail();
  assert(mode && mode->isOfType(NbViewerNavigationMode::getClassTypeId()));
  SbString modestring = mode->mode.getValue();
  path->unref();

  if (!modestring.getLength() > 0) return defsys;

  return NbNavigationSystem::getByName(modestring.getString());
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
    this->getbboxaction = new SoGetBoundingBoxAction(vp);
  } else {
    this->getbboxaction->setViewportRegion(vp);
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

Superimposition::Superimposition(SoNode * scene,
                                 SbBool enabled,
                                 SbBool autoredraw,
                                 SbBool zbufferon,
                                 SoSceneManager * manager)
{
  assert(scene != NULL);
  this->scene = scene;
  this->scene->ref();
  
  this->enabled = enabled;
  this->autoredraw = autoredraw;
  this->zbufferon = zbufferon;
  
  this->manager = manager;
  this->sensor = new SoNodeSensor(Superimposition::changeCB, this);
  this->sensor->attach(this->scene);
}

Superimposition::~Superimposition()
{
  delete this->sensor;
  this->scene->unref();
}

void 
Superimposition::render(void) 
{
  if (!this->enabled) return;
  
  this->zbufferwason = glIsEnabled(GL_DEPTH_TEST) ? TRUE : FALSE;
  
  this->zbufferon ?
    glEnable(GL_DEPTH_TEST):
    glDisable(GL_DEPTH_TEST);
  
  this->manager->getGLRenderAction()->apply(this->scene);
  
  this->zbufferwason ?
    glEnable(GL_DEPTH_TEST):
    glDisable(GL_DEPTH_TEST);
}

void 
Superimposition::changeCB(void * data, SoSensor * sensor) 
{
  Superimposition * thisp = (Superimposition *) data;
  assert(thisp && thisp->manager);
  if (thisp->autoredraw) {
    thisp->manager->scheduleRedraw();
  }
}
