#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = voronoi

include( ../base/base.pro )

TRANSLATIONS = voronoi_pt.ts \
			   voronoi_sp.ts

INCLUDEPATH += ./include

HEADERS +=  ./include/voronoidiagramplugin.h \
			./include/voronoidiagramPluginCode.h \
			./include/VoronoiDiagramGenerator.h \
			./include/VoronoiWindow.h \
			./include/Utils.h
			
SOURCES +=  ./src/voronoidiagramplugin.cpp \
			./src/voronoidiagramPluginCode.cpp \
			./src/VoronoiDiagramGenerator.cpp \
			./src/VoronoiWindow.cpp \
			./src/Utils.cpp
			
FORMS += ./ui/UIVoronoi.ui

IMAGES	= ./images/voronoi.png \
		  ./images/delaunay.png
