#ifndef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/nodes/SoSubNode.h>

#include <NutsnBolts/nodes/NbViewerNavigationMode.h>

// *************************************************************************

/*!
  \class NbViewerNavigationMode NutsnBolts/nodes/NbViewerNavigationMode.h
  \brief Class for setting the viewer navigation mode from a scene graph.

  In this context, a viewer navigation mode is the viewer navigation system,
  not a sub-mode.

  \ingroup nodes
  \ingroup navigation
*/

#define PRIVATE(obj) ((obj)->pimpl)

SO_NODE_SOURCE(NbViewerNavigationMode);

void
NbViewerNavigationMode::initClass(void)
{
  SO_NODE_INIT_CLASS(NbViewerNavigationMode, SoNode, SoNode);
}

NbViewerNavigationMode::NbViewerNavigationMode(void)
{
  PRIVATE(this) = NULL; // reserved for future
  SO_NODE_CONSTRUCTOR(NbViewerNavigationMode);
  SO_NODE_ADD_FIELD(mode, (""));
}

NbViewerNavigationMode::~NbViewerNavigationMode(void)
{
}

#undef PRIVATE
