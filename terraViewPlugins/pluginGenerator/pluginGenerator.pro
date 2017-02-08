include( ../base/base.pro )
TEMPLATE	= lib
LANGUAGE	= C++
TRANSLATIONS = pluginGenerator_pt.ts \
			pluginGenerator_es.ts

INCLUDEPATH += ./include

HEADERS += ./include/pluginGeneratorCode.h \
		./include/TVPLGplugin.h \
		./include/TVPLGCodeGenerator.h
		
SOURCES += ./src/pluginGeneratorCode.cpp \
		./src/TVPLGplugin.cpp \
		./src/TVPLGCodeGenerator.cpp

FORMS	= ui/plggenerator.ui
