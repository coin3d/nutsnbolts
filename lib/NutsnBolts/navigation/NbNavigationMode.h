#ifndef NB_NAVIGATIONMODE_H
#define NB_NAVIGATIONMODE_H

#include <Inventor/SbName.h>
#include <Inventor/SbViewportRegion.h>

#include <NutsnBolts/Basic.h>

class SbRotation;
class SoNode;
class SoCamera;
class SoEvent;

class NbNavigationSystem;
class NbNavigationInfo;

class NbNavigationModeP;

class NB_DLL_API NbNavigationMode {
public:
  static void initClass(void);
  static void cleanClass(void);

public:
  NbNavigationMode(NbNavigationSystem * system, SbName modename);
  virtual ~NbNavigationMode(void);

  SbName getModeName(void) const;
  NbNavigationSystem * getNavigationSystem(void) const;

  SbBool processEvent(const SoEvent * event, const NbNavigationInfo * info);

  virtual SoNode * getSceneGraph(void);

  void addTransition(NbNavigationMode * newmode,
		     const SoEvent * event,
		     SbBool stack = TRUE,
		     const SoEvent * condition = NULL);
  void addAbort(const SoEvent * event, const SoEvent * condition = NULL);
  void addFinish(const SoEvent * event, const SoEvent * condition = NULL);

  void activate(void);
  void deactivate(void);
  SbBool isActive(void) const;

  virtual SbBool handleEvent(const SoEvent * event, const NbNavigationInfo * info) = 0;

  virtual void init(const SoEvent * event, const NbNavigationInfo * info);
  virtual void abort(const SoEvent * event, const NbNavigationInfo * info);
  virtual void finish(const SoEvent * event, const NbNavigationInfo * info);

protected:
  SbVec2s getInitialPosition(void) const;
  SbVec2s getPreviousPosition(void) const;
  SbVec2s getCurrentPosition(void) const;

  SbVec2f getInitialNormalizedPosition(const NbNavigationInfo * info) const;
  SbVec2f getPreviousNormalizedPosition(const NbNavigationInfo * info) const;
  SbVec2f getCurrentNormalizedPosition(const NbNavigationInfo * info) const;

private:
  NbNavigationModeP * pimpl;

}; // NbNavigationMode

#endif // !NB_NAVIGATIONMODE_H
