#ifndef NB_VIEWERNAVIGATIONMODE_H
#define NB_VIEWERNAVIGATIONMODE_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoNode.h>

#include <Inventor/fields/SoSFString.h>

class NbViewerNavigationModeP;
class NbViewerNavigationMode : public SoNode {
  typedef SoNode inherited;
  SO_NODE_HEADER(NbViewerNavigationMode);

public:
  static void initClass(void);
  NbViewerNavigationMode(void);

  SoSFString mode;

protected:
  virtual ~NbViewerNavigationMode(void);

private:
  NbViewerNavigationModeP * pimpl;

}; // NbViewerNavigationMode

#endif // !NB_VIEWERNAVIGATIONMODE_H
