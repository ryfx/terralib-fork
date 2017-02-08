#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = KFunctions
include( ../base.pri )
TRANSLATIONS = kfunctions_pt.ts \
		kfunctions_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/kfuncplugin.h \
	./include/kfunctionsPluginCode.h \
	./include/curve.h \
	./include/envelope.h \
	./include/k12.h \
	./include/k.h \
	./include/KFunctionPluginWindow.h \
	./include/klabel.h \
	./include/LoadParams.h \
	./include/xyplot.h \
	./include/xyplotgraphic.h \
	./include/xyseries.h
SOURCES +=  ./src/kfuncplugin.cpp \
	./src/kfunctionsPluginCode.cpp \
	./src/curve.cpp \
	./src/envelope.cpp \
	./src/k12.cpp \
	./src/k.cpp \
	./src/KFunctionPluginWindow.cpp \
	./src/klabel.cpp \
	./src/LoadParams.cpp \
	./src/xyplot.cpp \
	./src/xyplotgraphic.cpp \
	./src/xyseries.cpp
FORMS += ./ui/KFunctionForm.ui \
		./ui/xyplotgraphic.ui