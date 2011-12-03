#ifndef NB_NAVIGATIONCONTROL_H
#define NB_NAVIGATIONCONTROL_H

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

#include <Inventor/SbVec2s.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/SbVec3d.h>

#include <NutsnBolts/Basic.h>

class SoPath;
class SoCamera;
class SoNode;
class SbRotation;
class SbViewportRegion;

class NbNavigationControlP;

class NB_DLL_API NbNavigationControl {
public:
  NbNavigationControl(void);
  virtual ~NbNavigationControl(void);

  void setSceneGraph(SoNode * scene);
  SoNode * getSceneGraph(void) const;

  void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;

  void setUpVector(const SbVec3f & up);
  const SbVec3f & getUpVector(void) const;

  void saveCamera(void) const;
  void restoreCamera(void) const;

  SoPath * pick(SbVec2s pos, SbVec3d & pickpos) const;

  void viewAll(void) const;

  void viewPart(SoPath * path,
                const SbVec3f & in, const SbVec3f & up) const;

  void pointDir(const SbVec3f & dir) const;

  void reorientCamera(const SbRotation & rot) const;
  void reorientCamera(const SbVec3f & pointat) const;
  void pitchCamera(float angle) const;
  void yawCamera(float angle) const;
  void rollCamera(float angle) const;
  void resetRoll(void) const;

  void moveCamera(const SbVec3f & distance) const;
  void moveCamera(float factor, const SbBool keepfocalpoint = TRUE) const;

  void setViewport(const SbViewportRegion & vp);
  SbVec2s getViewportSize(void) const;
  float getViewportAspect(void) const;

protected:

private:
  NbNavigationControlP * pimpl;

}; // NbNavigationControl

#endif  // !NB_NAVIGATIONCONTROL_H
