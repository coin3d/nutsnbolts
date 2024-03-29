#include <cassert>
#include <cstdio>

#include <Inventor/SoDB.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoRotor.h>
#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoDirectionalLight.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#include <Inventor/Wx/SoWx.h>
#include <Inventor/Wx/SoWxRenderArea.h>

#include <NutsnBolts/NutsnBolts.h>
#include <NutsnBolts/NbSceneManager.h>
#include <NutsnBolts/navigation/NbNavigationMode.h>
#include <NutsnBolts/navigation/NbNavigationSystem.h>

// *************************************************************************

void
modechange(void * closure, NbNavigationSystem * system)
{
  fprintf(stderr, "mode change: %s\n",
    system->getCurrentModeName().getString());
}

int
main(int argc, char ** argv)
{
  wxWindow *window = SoWx::init("test", "test");
  NutsnBolts::init();

  SoWxRenderArea * ra = new SoWxRenderArea(window);
  ra->show();
  SoWx::show(window);

  NbSceneManager * sm = new NbSceneManager;
  sm->activate();
  ra->setSceneManager(sm);

  sm->setNavigationState(NbSceneManager::JUST_NAVIGATION);

  // NbNavigationSystem * nav = NbNavigationSystem::getByName(NB_EXAMINER_SYSTEM);
  NbNavigationSystem * nav = NbNavigationSystem::getByName(NB_ZOOMER_SYSTEM);
  nav->addModeChangeCallback(modechange, NULL);
  sm->setNavigationSystem(nav);

  SoSeparator * scene = new SoSeparator;
  SoDirectionalLight * light = new SoDirectionalLight;
  SoPerspectiveCamera * camera = new SoPerspectiveCamera;
  light->direction.setValue(SbVec3f(0.5, 0.5, -1.0));
  light->intensity.setValue(1.0);
  SoRotor * rotor = new SoRotor;
  rotor->speed.setValue(0.5);
  scene->addChild(light);
  scene->addChild(camera);
  // scene->addChild(rotor);
  scene->addChild(new SoCube);

  camera->position.setValue(0, 0, -5);
  camera->pointAt(SbVec3f(0, 0, 0));
  // camera->viewAll(scene);

  ra->setSceneGraph(scene);

  SoWx::mainLoop();

  return 0;
}

// *************************************************************************
