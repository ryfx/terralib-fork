#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Jacquez_kNN
include( ../base.pri )
TRANSLATIONS = jacquez_knn_pt.ts \
		jacquez_knn_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/knnplugin.h \
	./include/jacquez_knnPluginCode.h \
	./include/funcoes.h \
	./include/kNNPluginWindow.h
SOURCES +=  ./src/knnplugin.cpp \
	./src/jacquez_knnPluginCode.cpp \
	./src/funcoes.cpp \
	./src/kNNPluginWindow.cpp
FORMS += ./ui/kNNForm.ui