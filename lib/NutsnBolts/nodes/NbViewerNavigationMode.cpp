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

#ifndef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <Inventor/nodes/SoSubNode.h>

#include <NutsnBolts/nodes/NbViewerNavigationMode.h>

// *************************************************************************

/*!
  \class NbViewerNavigationMode NutsnBolts/nodes/NbViewerNavigationMode.h
  \brief Node for setting the default viewer navigation mode for a scene graph.

  This node provides a way to hint to the NbSceneManager which
  navigation mode you want to use with a scene graph.

  The node must be placed before the first camera in the scene graph,
  or it will not be found by the system that searches for this node.

  It will only work in cooperation with the NbSceneManager.  If for
  instance just the regular SoSceneManager is used in the viewer, this
  node does absolutely nothing.

  Currently, the mode field of the node is not monitored for dynamic
  changes, so updating it will not change the viewer mode.  Doing this is
  a possibility for future enhancement.

  Be aware that "navigation mode" in this context translates to a
  complete navigation system (NbNavigationSystem), and not an atomic
  navigation submode (NbNavigationMode).

  \ingroup nodes
  \ingroup navigation
*/

/*!
  \var SoSFString NbViewerNavigationMode::mode

  The mode field is used for setting the name of the desired mode.
  If the mode is not found in the system, no mode will be used.

  The builtin modes are currently limited to "examiner", "panner",
  "zoomer", and "rotater".  In addition, application developers can
  extend the list with their own custom modes.
*/

#define PRIVATE(obj) ((obj)->pimpl)

SO_NODE_SOURCE(NbViewerNavigationMode);

/*!
  Static class initializer.  It is called from NutsnBolts::init, so
  there should be no need to call this directly.
*/

void
NbViewerNavigationMode::initClass(void)
{
  SO_NODE_INIT_CLASS(NbViewerNavigationMode, SoNode, SoNode);
}

/*!
  Constructor.
*/

NbViewerNavigationMode::NbViewerNavigationMode(void)
{
  PRIVATE(this) = NULL; // reserved for future
  SO_NODE_CONSTRUCTOR(NbViewerNavigationMode);
  SO_NODE_ADD_FIELD(mode, (""));
}

/*!
  Destructor.
*/

NbViewerNavigationMode::~NbViewerNavigationMode(void)
{
}

#undef PRIVATE
