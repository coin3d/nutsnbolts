#ifndef NB_SOEVENT_H
#define NB_SOEVENT_H

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

#ifndef NB_INTERNAL
#error NutsnBolts/misc/SoEvent.h is a private/internal header file!
#endif // !NB_INTERNAL

#include <Inventor/SbBasic.h>

class SoEvent;

SbBool SoEvent_Equals(const SoEvent * ev1, const SoEvent * ev2);
SoEvent * SoEvent_Clone(const SoEvent * ev);

#endif // !NB_SOEVENT_H
