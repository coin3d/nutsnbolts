#ifndef NB_NUTSNBOLTS_H
#define NB_NUTSNBOLTS_H

#include <NutsnBolts/Basic.h>

class NB_DLL_API NutsnBolts {
public:
  static void init(void);
  static void clean(void);

  static int getMajorVersion(void);
  static int getMinorVersion(void);
  static int getMicroVersion(void);
  static const char * getVersion(void);

}; // class NutsnBolts

#endif // !NB_NUTSNBOLTS_H
