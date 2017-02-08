include (../base/base.pro)

SOURCES += EDAPluginCode.cpp
SOURCES += functions/EDA_SharedFunctions.cpp
SOURCES += functions/EDA_Aspect.cpp
SOURCES += functions/EDA_SlopeDegrees.cpp
SOURCES += functions/EDA_SlopePercent.cpp
SOURCES += functions/TeTarDEM_SharedSubRotines.cpp
SOURCES += functions/TeTarDEM_Flood.cpp
SOURCES += functions/TeTarDEM_SetDir.cpp
HEADERS += EDAPluginCode.h
HEADERS += functions/EDA_SharedFunctions.h
HEADERS += functions/EDA_Aspect.h
HEADERS += functions/EDA_SlopeDegrees.h
HEADERS += functions/EDA_SlopePercent.h
HEADERS += functions/TeTarDEM.h
HEADERS += functions/TeTarDEM_SharedSubRotines.h
HEADERS += functions/TeTarDEM_Flood.h
HEADERS += functions/TeTarDEM_SetDir.h
FORMS	+= EDAPluginMainForm.ui