#ifndef NB_SCENEMANAGER_H
#define NB_SCENEMANAGER_H

#include <Inventor/SoSceneManager.h>

#include <NutsnBolts/NutsnBolts.h>

class SoCamera;
class NbNavigationSystem;
class NbNavigationMode;

class NbSceneManagerP;

class NB_DLL_API NbSceneManager : public SoSceneManager {
  typedef SoSceneManager inherited;

public:
  NbSceneManager(void);
  virtual ~NbSceneManager(void);

  virtual void render(const SbBool clearwindow = TRUE,
                      const SbBool clearzbuffer = TRUE);
  virtual void render(SoGLRenderAction * action,
                      const SbBool initmatrices = TRUE,
                      const SbBool clearwindow = TRUE,
                      const SbBool clearzbuffer = TRUE);
  
  virtual void setCamera(SoCamera * camera);
  SoCamera * getCamera(void) const;

  enum RenderMode {
    AS_IS,
    WIREFRAME,
    POINTS,
    WIREFRAME_OVERLAY,
    HIDDEN_LINE,
    BOUNDING_BOX
  };
  
  virtual void setRenderMode(RenderMode mode);
  RenderMode getRenderMode(void) const;

  enum StereoMode {
    MONO,
    RED_CYAN,
    RED_BLUE,
    QUAD_BUFFER,
    INTERLEAVED_ROWS,
    INTERLEAVED_COLUMNS
  };

  virtual void setStereoMode(StereoMode mode);
  StereoMode getStereoMode(void) const;

  virtual void setStereoOffset(float offset);
  float getStereoOffset(void) const;
  
  virtual void setTexturesEnabled(const SbBool onoff);
  SbBool isTexturesEnabled(void) const;

  virtual void setWireframeOverlayColor(const SbColor & color);
  const SbColor & getWireframeOverlayColor(void) const;

  virtual SbBool processEvent(const SoEvent * const event);

  enum NavigationState {
    NO_NAVIGATION,
    JUST_NAVIGATION,
    MIXED_NAVIGATION
  };

  virtual void setNavigationState(NavigationState state);
  virtual NavigationState getNavigationState(void) const;

  virtual void setNavigationSystem(NbNavigationSystem * system);
  virtual NbNavigationSystem * getNavigationSystem(void) const;

  virtual void setSceneGraph(SoNode * const root);
  virtual SoNode * getSceneGraph(void) const;

  virtual void activate(void);
  virtual void deactivate(void);

  enum AutoClippingStrategy {
    NO_AUTO_CLIPPING,
    FIXED_NEAR_PLANE,
    VARIABLE_NEAR_PLANE
  };

  virtual void setAutoClipping(AutoClippingStrategy strategy);
  virtual AutoClippingStrategy getAutoClipping(void) const;

  virtual void setNearPlaneValue(float value);
  virtual float getNearPlaneValue(void) const;

private:
  NbSceneManagerP * pimpl;

}; // NbSceneManager

#endif // !NB_SCENEMANAGER_H
