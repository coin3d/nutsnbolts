
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>

#include <Inventor/SoDB.h>

#include <NutsnBolts/NutsnBolts.h>
#include <NutsnBolts/nodes/NbViewerNavigationMode.h>
#include <NutsnBolts/navigation/NbNavigationSystem.h>
#include <NutsnBolts/navigation/NbNavigationMode.h>

/*!
  \mainpage Nuts'n'Bolts v0.1

  The SIM Nuts'n'Bolts library is a library of useful utilities for
  viewer/applications developers.  The collection of classes is viewer-
  centric - you will not find nodes more suitable for other engineering-
  domains in this library.

  This library might not be upward compatible between major release
  versions.  Users of this library are therefore encouraged to use the
  NB_VERSION_# define heavily to write code that can be built against
  multiple versions of SIM Nuts'n'Bolts the APIs change.
*/

/*!
  \defgroup nodes
*/

/*!
  \defgroup navigation

  This group is the modules the navigation system consists of.
*/

/*!
  \defgroup misc
*/

/*!
  \defgroup internal
  \brief Internal stuff.
*/

/*!
  \class NutsnBolts NutsnBolts/NutsnBolts.h
  \brief Static namespace class for global funtions in the SIM Nuts'n'Bolts
  library.

  \ingroup misc
*/

/*!
  This function initializes all the Nuts'n'Bolts components.
*/

void
NutsnBolts::init(void)
{
  NbViewerNavigationMode::initClass();
  NbNavigationMode::initClass();
  NbNavigationSystem::initClass();
#if 0
  coin_atexit(NutsnBolts::Clean);
#endif
}

/*!
  This function cleans up after all the Nuts'n'Bolts components.
*/

void
NutsnBolts::clean(void)
{
  NbNavigationSystem::cleanClass();
}

/*!
  This function returns the major version number (!.*.*) of the library.
*/

int
NutsnBolts::getMajorVersion(void)
{
  return NB_MAJOR_VERSION;
}

/*!
  This function returns the minor version number (*.!.*) of the library.
*/

int
NutsnBolts::getMinorVersion(void)
{
  return NB_MINOR_VERSION;
}

/*!
  This function returns the micro version number (*.*.!) of the library.
*/

int
NutsnBolts::getMicroVersion(void)
{
  return NB_MICRO_VERSION;
}

/*!
  This function returns the version string of the library.
*/

const char *
NutsnBolts::getVersion(void)
{
  static const char version[] = NB_VERSION;
  return version;
}

// *************************************************************************
