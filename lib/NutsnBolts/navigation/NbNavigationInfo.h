#ifndef NB_NAVIGATIONINFO_H
#define NB_NAVIGATIONINFO_H

#include <Inventor/SbVec2s.h>

#include <NutsnBolts/Basic.h>

class SoCamera;
class SbRotation;
class SbViewportRegion;

class NbNavigationInfoP;

class NB_DLL_API NbNavigationInfo {
public:
  NbNavigationInfo(void);
  virtual ~NbNavigationInfo(void);

  void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;

  void reorientCamera(const SbRotation & rot) const;
  void moveCamera(const SbVec3f & vec) const;
  void setCamera(void) const;
  void restoreCamera(void) const;

  void setViewport(const SbViewportRegion & vp);
  SbVec2s getViewportSize(void) const;
  float getViewportAspect(void) const;

protected:

private:
  NbNavigationInfoP * pimpl;

}; // NbNavigationInfo

#endif  // !NB_NAVIGATIONINFO_H
