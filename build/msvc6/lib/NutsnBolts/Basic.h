/* lib/NutsnBolts/Basic.h.  Generated from Basic.h.in by configure.  */
#ifndef NB_BASIC_H
#define NB_BASIC_H

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

#define NB_MAJOR_VERSION 0
#define NB_MINOR_VERSION 1
#define NB_MICRO_VERSION 0
/* #undef NB_BETA_VERSION */
#define NB_VERSION "0.1.0a"

#define NB_VERSION_0
#define NB_VERSION_0_1

/*
  See Coin's Inventor/C/basic.h for an explanation to the below #define
  setup.
*/

#ifdef NB_DLL_API
#error Leave the NB_DLL_API define alone
#endif // NB_MAKE_DLL

#ifdef NB_INTERNAL
# ifdef NB_NOT_DLL
#  error The NB_DLL define should not be used when building the library - only when building win32 applications.
# endif // NB_NOT_DLL
# ifdef NB_DLL
#  error The NB_NOT_DLL define should not be used when building the library - only when building win32 applications.
# endif // NB_DLL
#endif // NB_INTERNAL

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
# ifdef NB_INTERNAL
#  ifdef NB_MAKE_DLL
#   define NB_DLL_API __declspec(dllexport)
#  endif // NB_MAKE_DLL
# else // !NB_INTERNAL
#  ifdef NB_DLL
#   ifdef NB_NOT_DLL
#    error Define _either_ NB_DLL or NB_NOT_DLL as appropriate for your linkage -- not both at the same time! See the Coin Inventor/C/basic.h for further instructions.
#   endif // NB_NOT_DLL
#   define NB_DLL_API __declspec(dllimport)
#  else // !NB_DLL
#   ifndef NB_NOT_DLL
#    error Define either NB_DLL or NB_NOT_DLL as appropriate for your linkage. See the Coin Inventor/C/basic.h for further instructions.
#   endif // !NB_NOT_DLL
#  endif // !NB_DLL
# endif // !NB_INTERNAL
#endif // Microsoft Windows

#ifndef NB_DLL_API
# define NB_DLL_API
#endif // !NB_DLL_API

#endif // !NB_BASIC_H
