#ifndef NB_NAVIGATIONSYSTEM_H
#define NB_NAVIGATIONSYSTEM_H

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
#include <Inventor/SbVec3f.h>

#include <NutsnBolts/Basic.h>

class SbViewportRegion;
class SoNode;
class SoPath;
class SoCamera;
class SoEvent;

class NbNavigationMode;
class NbNavigationSystem;
class NbNavigationControl;

#define NB_IDLER_SYSTEM                 "idler"
#define NB_IDLER_IDLE_MODE              "idler:idle"

#define NB_EXAMINER_SYSTEM              "examiner"
#define NB_EXAMINER_IDLE_MODE           "examiner:idle"
#define NB_EXAMINER_ROTATE_MODE         "examiner:rotate"
#define NB_EXAMINER_WAITFORZOOM_MODE    "examiner:wait_for_zoom"
#define NB_EXAMINER_ZOOM_MODE           "examiner:zoom"
#define NB_EXAMINER_WAITFORPAN_MODE     "examiner:wait_for_pan"
#define NB_EXAMINER_PAN_MODE            "examiner:pan"
#define NB_EXAMINER_WAITFORCENTER_MODE  "examiner:wait_for_center"
#define NB_EXAMINER_CENTER_MODE         "examiner:center"

// some simple button1-modes
#define NB_PANNER_SYSTEM                "panner"
#define NB_PANNER_IDLE_MODE             "panner:idle"
#define NB_PANNER_PAN_MODE              "panner:pan"

#define NB_ZOOMER_SYSTEM                "zoomer"
#define NB_ZOOMER_IDLE_MODE             "zoomer:idle"
#define NB_ZOOMER_ZOOM_MODE             "zoomer:zoom"

#define NB_ROTATER_SYSTEM               "rotater"
#define NB_ROTATER_IDLE_MODE            "rotater:idle"
#define NB_ROTATER_ROTATE_MODE          "rotater:rotate"

#define NB_CENTERER_SYSTEM              "centerer"
#define NB_CENTERER_IDLE_MODE           "centerer:idle"
#define NB_CENTERER_CENTER_MODE         "centerer:center"

#define NB_PICKER_SYSTEM                "picker"
#define NB_PICKER_IDLE_MODE             "picker:idle"
#define NB_PICKER_PICK_MODE             "picker:pick"

#define NB_DEFAULT_SYSTEM               NB_EXAMINER_SYSTEM

typedef
  void NbNavigationModeChangeCB(void * closure, NbNavigationSystem * system);

class NbNavigationSystemP;

class NB_DLL_API NbNavigationSystem {
public:
  static void initClass(void);
  static void cleanClass(void);

  static SbBool registerSystem(NbNavigationSystem * system);
  static SbBool unregisterSystem(NbNavigationSystem * system);
  static SbBool isRegistered(NbNavigationSystem * system);

  static NbNavigationSystem * getByName(SbName name);
  static NbNavigationSystem * createByName(SbName name);

public:
  NbNavigationSystem(SbName name);
  ~NbNavigationSystem(void);

  NbNavigationSystem * clone(void) const;

  void addModeChangeCallback(NbNavigationModeChangeCB * cb, void * closure);
  void removeModeChangeCallback(NbNavigationModeChangeCB * cb, void * closure);

  void setSceneGraph(SoNode * scenegraph);
  void setCamera(SoCamera * camera);
  void setViewport(const SbViewportRegion & viewport);

  void viewAll(void);
  void viewPart(SoPath * path, const SbVec3f & in, const SbVec3f & up);
  void viewPart(SoNode * node, const SbVec3f & in, const SbVec3f & up);

  SbBool processEvent(const SoEvent * event);
  NbNavigationControl * getNavigationControl(void) const;

  SbName getName(void) const;

  SbName getCurrentModeName(void) const;
  NbNavigationMode * getMode(SbName name) const;
  NbNavigationMode * getCurrentMode(void) const;
  
  enum TransitionType {
    INITIAL,
    STACK,
    SWITCH,
    FINISH,
    ABORT
  };

  void addMode(NbNavigationMode * mode);
  void addModeTransition(NbNavigationMode * mode,
			 TransitionType type,
			 const SoEvent * trigger = NULL,
			 const SoEvent * condition = NULL);
  void addModeTransition(NbNavigationMode * mode1,
			 NbNavigationMode * mode2,
			 TransitionType type,
			 const SoEvent * trigger,
			 const SoEvent * condition = NULL);

protected:
  friend class NbSceneManager;
  void invokeModeChangeCallbacks(void);

private:
  NbNavigationSystemP * pimpl;

}; // NbNavigationSystem

#endif // !NB_NAVIGATIONSYSTEM_H
