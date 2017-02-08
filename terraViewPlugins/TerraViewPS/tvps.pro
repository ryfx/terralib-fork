#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------

TARGET = tvps

include(../base/base.pro)

TRANSLATIONS = tvps_pt.ts

INCLUDEPATH += ./include

HEADERS +=  ./include/tvpsplugin.h \
			./include/tvpsPluginCode.h \
			./include/Shortcut.h \
			./include/DatabaseShortcut.h \
			./include/DatabaseToolBar.h \
			./include/DatabaseMenu.h \
			./include/LayerShortcut.h \
			./include/LayerToolBar.h \
			./include/LayerMenu.h \
			./include/ViewShortcut.h \
			./include/ViewToolBar.h \
			./include/ViewMenu.h \
			./include/ThemeShortcut.h \
			./include/ThemeToolBar.h \
			./include/ThemeMenu.h \
			./include/QueryShortcut.h \
			./include/QueryToolBar.h \
			./include/TableShortcut.h \
			./include/TableMenu.h \
			./include/ThematicMappingMenu.h \
			./include/DisplayShortcut.h \
			./include/DisplayMenu.h \
			./include/ShowMenu.h
			
SOURCES +=  ./src/tvpsplugin.cpp \
			./src/tvpsPluginCode.cpp \
			./src/Shortcut.cpp \
			./src/DatabaseShortcut.cpp \
			./src/DatabaseToolBar.cpp \
			./src/DatabaseMenu.cpp \
			./src/LayerShortcut.cpp \
			./src/LayerToolBar.cpp \
			./src/LayerMenu.cpp \
			./src/ViewShortcut.cpp \
			./src/ViewToolBar.cpp \
			./src/ViewMenu.cpp \
			./src/ThemeShortcut.cpp \
			./src/ThemeToolBar.cpp \
			./src/ThemeMenu.cpp \
			./src/QueryShortcut.cpp \
			./src/QueryToolBar.cpp \
			./src/TableShortcut.cpp \
			./src/TableMenu.cpp \
			./src/ThematicMappingMenu.cpp \
			./src/DisplayShortcut.cpp \
			./src/DisplayMenu.cpp \
			./src/ShowMenu.cpp
			
IMAGES  = ./images/attrQuery.png \
		  ./images/databaseRefresh.png \
		  ./images/databaseInfo.png \
		  ./images/layerInfo.png \
		  ./images/layerProjection.png \
		  ./images/layerRemove.png \
		  ./images/layerRename.png \
		  ./images/spatialQuery.png \
		  ./images/themeInfo.png \
		  ./images/themeRemove.png \
		  ./images/themeRename.png \
		  ./images/themeVisual.png \
		  ./images/viewInfo.png \
		  ./images/viewProjection.png \
		  ./images/viewRemove.png \
		  ./images/viewRename.png \
		  ./images/check.png \
		  ./images/uncheck.png