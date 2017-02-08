#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Rogerson
include( ../base.pri )
TRANSLATIONS = rogerson_pt.ts \
		rogerson_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/rogersonplugin.h \
	./include/rogersonPluginCode.h \
	./include/algoritmos.h \
	./include/curve.h \
	./include/RogersonPluginWindow.h \
	./include/xyplot.h \
	./include/xyplotgraphic.h \
	./include/xyseries.h
SOURCES +=  ./src/rogersonplugin.cpp \
	./src/rogersonPluginCode.cpp \
	./src/algoritmos.cpp \
	./src/curve.cpp \
	./src/RogersonPluginWindow.cpp \
	./src/xyplot.cpp \
	./src/xyplotgraphic.cpp \
	./src/xyseries.cpp
FORMS += ./ui/RogersonForm.ui \
		./ui/xyplotgraphic.ui