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
