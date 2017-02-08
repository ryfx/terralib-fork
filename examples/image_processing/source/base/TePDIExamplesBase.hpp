#ifndef TEPDIEXAMPLESBASE_HPP
  #define TEPDIEXAMPLESBASE_HPP

  #include <terralib/kernel/TeDefines.h>

  #if TePLATFORM == TePLATFORMCODE_MSWINDOWS
    #define TEPDIEXAMPLESRESPATH "..\\..\\resources\\"
    #define TEPDIEXAMPLESBINPATH "..\\..\\bin\\"
  #elif TePLATFORM == TePLATFORMCODE_LINUX
    #ifdef TE_DATA_PDIEXAMPLES_LOCALE
      #define TEPDIEXAMPLESRESPATH TE_DATA_PDIEXAMPLES_LOCALE
      #define TEPDIEXAMPLESBINPATH TE_DATA_PDIEXAMPLES_BIN
    #else
      #define TEPDIEXAMPLESRESPATH "../resources/"
      #define TEPDIEXAMPLESBINPATH "../bin/"
    #endif
  #elif TePLATFORM == TePLATFORMCODE_APPLE
    #ifdef TE_DATA_PDIEXAMPLES_LOCALE
      #define TEPDIEXAMPLESRESPATH TE_DATA_PDIEXAMPLES_LOCALE
      #define TEPDIEXAMPLESBINPATH TE_DATA_PDIEXAMPLES_BIN
    #else
      #define TEPDIEXAMPLESRESPATH "../resources/"
      #define TEPDIEXAMPLESBINPATH "../bin/"
    #endif    
  #else
    #error "ERROR: Unsupported platform"
  #endif

#endif
