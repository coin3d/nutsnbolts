#ifndef NB_SUBMODE_H
#define NB_SUBMODE_H

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

#include <assert.h>
#include <string.h>

#include <Inventor/SoType.h>

#define NB_NAVIGATION_MODE_HEADER(classname) \
public: \
  static SoType classTypeId; \
  static SoType getClassTypeId(void); \
  virtual SoType getTypeId(void) const

#define NB_NAVIGATION_MODE_SOURCE(classname) \
SoType classname::getTypeId(void) const { \
  return classname::classTypeId; \
} \
SoType classname::getClassTypeId(void) { \
  return classname::classTypeId; \
} \
SoType classname::classTypeId = SoType::badType()

#define NB_NAVIGATION_MODE_INIT_CLASS(classname, parentname) \
  do { \
    assert(classname::classTypeId == SoType::badType() && "don't init() twice!"); \
    /* Make sure superclass gets initialized before subclass. */ \
    assert(strcmp(SO__QUOTE(parentname), "inherited")); \
    SoType parentType(SoType::fromName(SO__QUOTE(parentname))); \
    assert(parentType != SoType::badType() && "you forgot init() on parentclass!"); \
 \
    /* Set up entry in the type system. */ \
    classname::classTypeId = \
      SoType::createType(parentType, \
                         SO__QUOTE(classname), \
                         NULL /* createfunc */, \
                         0 /* data */); \
  } while (0)

#endif // !NB_SUBMODE_H
