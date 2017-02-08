#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Grimson
include( ../base.pri )
TRANSLATIONS = grimson_pt.ts \
		grimson_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/grimsonplugin.h \
	./include/grimsonPluginCode.h \
	./include/curve.h \
	./include/fatorial.h \
	./include/GrimsonPluginWindow.h \
	./include/normal01.h \
	./include/poisson.h \
	./include/xyplot.h \
	./include/xyplotgraphic.h \
	./include/xyseries.h
SOURCES +=  ./src/grimsonplugin.cpp \
	./src/grimsonPluginCode.cpp \
	./src/curve.cpp \
	./src/GrimsonPluginWindow.cpp \
	./src/xyplot.cpp \
	./src/xyplotgraphic.cpp \
	./src/xyseries.cpp
FORMS += ./ui/GrimsonForm.ui \
		./ui/xyplotgraphic.ui