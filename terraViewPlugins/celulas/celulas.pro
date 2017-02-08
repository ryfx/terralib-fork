include (../base/base.pro)

SOURCES	+= CelulasWindow.cpp \
	CelulaPluginCode.cpp \
	../../src/terralib/functions/TeCellAlgorithms.cpp \
	../../src/terralib/functions/TeCoordAlgorithms.cpp \

HEADERS	+= CelulasWindow.h \
	CelulaPluginCode.h


FORMS	= celulasform.ui \
	help.ui
IMAGES	= images/terralib.png \
	images/reset.bmp \
	images/previousDisplay.bmp \
	images/nextDisplay.bmp

TRANSLATIONS =  celulas_pt.ts
