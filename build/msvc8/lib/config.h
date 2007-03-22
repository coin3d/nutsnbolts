#ifndef NUTSNBOLTS_DEBUG
#error The define NUTSNBOLTS_DEBUG needs to be defined to true or false
#endif

#if NUTSNBOLTS_DEBUG
#include "config-debug.h"
#else /* !NUTSNBOLTS_DEBUG */
#include "config-release.h"
#endif /* !NUTSNBOLTS_DEBUG */
