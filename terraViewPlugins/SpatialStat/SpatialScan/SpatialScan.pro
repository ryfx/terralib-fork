#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = SpatialScan
include( ../base.pri )
TRANSLATIONS = spatialscan_pt.ts \
		spatialscan_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/spscanplugin.h \
	./include/spatialscanPluginCode.h \
	./include/cluster.h \
	./include/SpatialScanPluginWindow.h
SOURCES +=  ./src/spscanplugin.cpp \
	./src/spatialscanPluginCode.cpp \
	./src/cluster.cpp \
	./src/SpatialScanPluginWindow.cpp
FORMS += ./ui/SpatialScanForm.ui