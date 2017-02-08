#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = tviewexternaltheme
win32:!win32-g++:TARGET = ExternalTheme
include( ../base/base.pro )
TEMPLATE	= lib
LANGUAGE	= C++
TRANSLATIONS = externalTheme_pt.ts \
			externalTheme_es.ts
INCLUDEPATH += ./include \
			$$TERRALIBPATH/src/terralib/utils
win32:!win32-g++:QMAKE_LIBDIR += $$TERRALIBPATH/$$CONFIGDIR/te_utils
LIBS += -lte_utils			
HEADERS +=  ./include/externalthemePlugin.h \
	./include/externalthemePluginCode.h \
	./include/externalDBThemeWindow.h
SOURCES +=  ./src/externalthemePlugin.cpp \
	./src/externalthemePluginCode.cpp \
	./src/externalDBThemeWindow.cpp
FORMS += ui/externalDBTheme.ui
IMAGES += $$TERRALIBPATH/terraView/ui/images/externalTheme.xpm