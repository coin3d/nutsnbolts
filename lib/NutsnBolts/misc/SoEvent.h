#ifndef NB_SOEVENT_H
#define NB_SOEVENT_H

#ifndef NB_INTERNAL
#error NutsnBolts/misc/SoEvent.h is a private/internal header file!
#endif // !NB_INTERNAL

#include <Inventor/SbBasic.h>

class SoEvent;

SbBool SoEvent_Equals(const SoEvent * ev1, const SoEvent * ev2);
SoEvent * SoEvent_Clone(const SoEvent * ev);

#endif // !NB_SOEVENT_H
