#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = IMoran
include( ../base.pri )
TRANSLATIONS = imoran_pt.ts \
		imoran_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/imoranplugin.h \
	./include/imoranPluginCode.h \
	./include/EBI.h \
	./include/IMoranPluginWindow.h
SOURCES +=  ./src/imoranplugin.cpp \
	./src/imoranPluginCode.cpp \
	./src/EBI.cpp \
#	./src/IMoran.cpp \
	./src/IMoranPluginWindow.cpp
FORMS += ./ui/IMoranForm.ui