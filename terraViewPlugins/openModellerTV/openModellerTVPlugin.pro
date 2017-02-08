include( ../base/base.pro )

FORMS += openModellerTVPluginWindow.ui \
         openModellerForm.ui \
		 parametersForm.ui \
		 progressForm.ui

HEADERS += openModellerTVPluginCode.h \
           openModellerPluginWindow.h \
		   parametersWindow.h \
		   QLogCallback.h \
		   QOpenModeller.h \
		   

SOURCES += openModellerTVPluginCode.cpp \
           openModellerPluginWindow.cpp \
		   parametersWindow.cpp \
		   QOpenModeller.cpp

DEFINES += dllexp=__declspec(dllimport)

INCLUDEPATH += libs\include

LIBS += libs\$$LIBSSUBDIRINCLUDE\lib\openmodeller.lib