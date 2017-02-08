#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Bithell
include( ../base.pri )
TRANSLATIONS = bithell_pt.ts \
		bithell_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/bithellplugin.h \
	./include/bithellPluginCode.h \
	./include/Bithell2.h \
	./include/BithellPluginWindow.h
SOURCES +=  ./src/bithellplugin.cpp \
	./src/bithellPluginCode.cpp \
	./src/Bithell2.cpp \
	./src/BithellPluginWindow.cpp
FORMS += ./ui/BithellForm.ui