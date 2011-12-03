#ifndef NB_NAVIGATIONMODE_H
#define NB_NAVIGATIONMODE_H

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

#include <Inventor/SbName.h>
#include <Inventor/SbVec2f.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbViewportRegion.h>

#include <NutsnBolts/Basic.h>
#include <NutsnBolts/navigation/NbSubMode.h>

class SbRotation;
class SoNode;
class SoCamera;
class SoEvent;

class NbNavigationSystem;
class NbNavigationControl;

class NbNavigationMode;
class NbNavigationModeP;

typedef float NbNavigation1DInputValueFunc(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
typedef SbVec2f NbNavigation2DInputValueFunc(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
typedef SbVec3f NbNavigation3DInputValueFunc(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);

class NB_DLL_API NbNavigationMode {
  NB_NAVIGATION_MODE_HEADER(NbNavigationMode);

public:
  static void initClass(void);

  // predefined set of input value functions
  static float getAbsMouseMovePixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveVerticalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveVerticalNormalizedDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveHorizontalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveHorizontalNormalizedDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getAbsMouseCenterPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseCenterVerticalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseCenterHorizontalPixelDistance(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseCenterAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);
  static float getMouseMoveCenterAngle(void * closure, const NbNavigationMode * mode, const NbNavigationControl * ctrl);

public:
  NbNavigationMode(SbName modename);
  virtual ~NbNavigationMode(void);

  virtual NbNavigationMode * clone(void) const = 0;

  SbName getModeName(void) const;

  void set1DValueFunc(NbNavigation1DInputValueFunc * func, void * closure);
  NbNavigation1DInputValueFunc * get1DValueFunc(void ** closureptr = NULL) const;
  void set2DValueFunc(NbNavigation2DInputValueFunc * func, void * closure);
  NbNavigation2DInputValueFunc * get2DValueFunc(void ** closureptr = NULL) const;
  void set3DValueFunc(NbNavigation3DInputValueFunc * func, void * closure);
  NbNavigation3DInputValueFunc * get3DValueFunc(void ** closureptr = NULL) const;

  SbBool processEvent(const SoEvent * event, const NbNavigationControl * ctrl);

  virtual SoNode * getSceneGraph(void);

  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationControl * ctrl) = 0;

  virtual void init(const SoEvent * event, const NbNavigationControl * ctrl);
  virtual void abort(const SoEvent * event, const NbNavigationControl * ctrl);
  virtual void finish(const SoEvent * event, const NbNavigationControl * ctrl);

  SbBool isAborted(void) const;
  SbBool isFinished(void) const;

protected:
  float get1DValue(const NbNavigationControl * ctrl) const;
  SbVec2f get2DValue(const NbNavigationControl * ctrl) const;
  SbVec3f get3DValue(const NbNavigationControl * ctrl) const;

  SbVec2s getInitialPosition(void) const;
  SbVec2s getPreviousPosition(void) const;
  SbVec2s getCurrentPosition(void) const;

  SbVec2f getInitialNormalizedPosition(const NbNavigationControl * ctrl) const;
  SbVec2f getPreviousNormalizedPosition(const NbNavigationControl * ctrl) const;
  SbVec2f getCurrentNormalizedPosition(const NbNavigationControl * ctrl) const;

  void abort(void);
  void finish(void);

private:
  NbNavigationModeP * pimpl;

}; // NbNavigationMode

#endif // !NB_NAVIGATIONMODE_H
