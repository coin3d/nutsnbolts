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
