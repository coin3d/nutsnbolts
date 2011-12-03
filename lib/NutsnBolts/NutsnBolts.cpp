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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Inventor/SoDB.h>

#include <NutsnBolts/NutsnBolts.h>
#include <NutsnBolts/nodes/NbViewerNavigationMode.h>
#include <NutsnBolts/navigation/NbNavigationSystem.h>
#include <NutsnBolts/navigation/NbNavigationMode.h>
#include <NutsnBolts/navigation/NbCenterMode.h>
#include <NutsnBolts/navigation/NbIdleMode.h>
#include <NutsnBolts/navigation/NbOrthoCenterMode.h>
#include <NutsnBolts/navigation/NbPanMode.h>
#include <NutsnBolts/navigation/NbPickMode.h>
#include <NutsnBolts/navigation/NbPitchMode.h>
#include <NutsnBolts/navigation/NbRollMode.h>
#include <NutsnBolts/navigation/NbRotateMode.h>
#include <NutsnBolts/navigation/NbYawMode.h>
#include <NutsnBolts/navigation/NbZoomMode.h>
#include <NutsnBolts/misc/NbNamedEvent.h>
#include <NutsnBolts/nodes/NbEventNode.h>
#include <NutsnBolts/nodes/NbScriptEvent.h>

// *************************************************************************

/*!
  \mainpage Nuts'n'Bolts v0.1

  The SIM Nuts'n'Bolts library is a library of useful utilities for
  viewer/applications developers.  The collection of classes is viewer-
  centric - you will not find nodes more suitable for any specific
  engineering-domains in this library.

  This library might not be upward compatible between major release
  versions.  Users of this library are therefore encouraged to use the
  NB_VERSION_# defines heavily to write code that can be built against
  multiple major versions of SIM Nuts'n'Bolts the APIs change.

  When using the SIM Nuts'n'Bolts library, the first thing you need to
  do in your application is to initialize the SIM Nuts'n'Bolts library.

  \code
  #include <NutsnBolts/NutsnBolts.h>
  ...
  NutsnBolts::init();
  \endcode

  Otherwise, you will not have initialized the SIM Nuts'n'Bolts class
  types (registered them in the Open Inventor type system), and you
  will get warning messages about bad types and such.
*/

/*!
  \defgroup nodes
  \brief Scene graph nodes.

  The scene graph nodes you will find in SIM Nuts'n'Bolts.
*/

/*!
  \defgroup navigation
  \brief Camera navigation system.

  The classes related to user-interactive camera navigation.
*/

/*!
  \defgroup misc
  \brief Miscellaneous.

  The classes and files that don't belong to any particular group.
*/

/*!
  \defgroup internal
  \brief Internal code.

  The classes and files that are not part of the public API.
*/

/*!
  \file NutsnBolts/Basic.h
  \brief Basic definitions used by the SIM Nuts'n'Bolts library.

  This file defines the API export defines that makes sure the
  public methods are exported from Windows DLLs, and later linked in
  correctly when linking against the SIM Nuts'n'Bolts library.

  It also contains defines that tell which version of the SIM
  Nuts'n'Bolts library that has been installed.
*/

/*!
  \class NutsnBolts NutsnBolts/NutsnBolts.h
  \brief Static namespace class for global funtions related to the
  SIM Nuts'n'Bolts library on the whole.

  This class contains just static functions that relate to library
  initialization and cleanup, and library versioning.

  \ingroup misc
*/

/*!
  This function initializes the SIM Nuts'n'Bolts library - all the
  classes and components.

  It calls all the other static class initializers in the library,
  making sure there is no need to call any other initializer than this
  to use SIM Nuts'n'Bolts.

  \sa NutsnBolts::clean
*/

void
NutsnBolts::init(void)
{
  NbViewerNavigationMode::initClass();
  NbNavigationSystem::initClass();
  NbNavigationMode::initClass();
  NbCenterMode::initClass();
  NbIdleMode::initClass();
  NbOrthoCenterMode::initClass();
  NbPanMode::initClass();
  NbPickMode::initClass();
  NbPitchMode::initClass();
  NbRollMode::initClass();
  NbRotateMode::initClass();
  NbYawMode::initClass();
  NbZoomMode::initClass();
  NbNamedEvent::initClass();
  NbEventNode::initClass();
  NbScriptEvent::initClass();

#if 0
  // FIXME: Put back before release (kintel 20050310)
  coin_atexit(NutsnBolts::Clean);
#endif
}

/*!
  This function cleans up after all the Nuts'n'Bolts components.

  \sa NutsnBolts::init
*/

void
NutsnBolts::clean(void)
{
  NbNavigationSystem::cleanClass();
}

/*!
  This function returns the major version number (!.*.*) of the library.

  \sa NutsnBolts::getMinorVersion, NutsnBolts::getMicroVersion, NutsnBolts::getVersion
*/

int
NutsnBolts::getMajorVersion(void)
{
  return NB_MAJOR_VERSION;
}

/*!
  This function returns the minor version number (*.!.*) of the library.

  \sa NutsnBolts::getMajorVersion, NutsnBolts::getMicroVersion, NutsnBolts::getVersion
*/

int
NutsnBolts::getMinorVersion(void)
{
  return NB_MINOR_VERSION;
}

/*!
  This function returns the micro version number (*.*.!) of the library.

  \sa NutsnBolts::getMajorVersion, NutsnBolts::getMinorVersion, NutsnBolts::getVersion
*/

int
NutsnBolts::getMicroVersion(void)
{
  return NB_MICRO_VERSION;
}

/*!
  This function returns the version string for the library version.

  \sa NutsnBolts::getMajorVersion, NutsnBolts::getMinorVersion, NutsnBolts::getMicroVersion
*/

const char *
NutsnBolts::getVersion(void)
{
  static const char version[] = NB_VERSION;
  return version;
}

// *************************************************************************
