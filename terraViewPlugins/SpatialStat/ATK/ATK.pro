#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = ATK
include( ../base.pri )
TRANSLATIONS = atk_pt.ts \
		atk_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/atkplugin.h \
	./include/atkPluginCode.h \
	./include/ATKPluginWindow.h \
	./include/ClusterAlg.h \
	./include/Clustering.h \
	./include/Event.h \
	./include/Geometry.h \
	./include/ProspectiveAnalysis.h \
	./include/Resource.h \
	./include/Scan.h \
	./include/ScanInt.h \
	./include/util.h
SOURCES +=  ./src/atkplugin.cpp \
	./src/atkPluginCode.cpp \
	./src/ATKPluginWindow.cpp \
	./src/ClusterAlg.cpp \
	./src/Event.cpp \
	./src/ProspectiveAnalysis.cpp \
	./src/ScanInt.cpp \
	./src/util.cpp
FORMS += ./ui/ATKForm.ui