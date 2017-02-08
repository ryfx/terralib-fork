#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = SR
include( ../base.pri )
TRANSLATIONS = sr_pt.ts \
		sr_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/srplugin.h \
	./include/srPluginCode.h \
	./include/algoritmos.h \
	./include/curve.h \
	./include/SRPluginWindow.h \
	./include/sv.h \
	./include/xyplot.h \
	./include/xyplotgraphic.h \
	./include/xyseries.h 
SOURCES +=  ./src/srplugin.cpp \
	./src/srPluginCode.cpp \
	./src/algoritmos.cpp \
	./src/curve.cpp \
	./src/SRPluginWindow.cpp \
	./src/sv.cpp \
	./src/xyplot.cpp \
	./src/xyplotgraphic.cpp \
	./src/xyseries.cpp
FORMS += ./ui/SRForm.ui \
		./ui/xyplotgraphic.ui