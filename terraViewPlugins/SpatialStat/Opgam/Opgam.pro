#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Opgam
include( ../base.pri )
TRANSLATIONS = opgam_pt.ts \
		opgam_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/opgamplugin.h \
	./include/opgamPluginCode.h \
	./include/lgamma.h \
	./include/OpgamPluginWindow.h
SOURCES +=  ./src/opgamplugin.cpp \
	./src/opgamPluginCode.cpp \
	./src/lgamma.cpp \
	./src/OpgamPluginWindow.cpp
FORMS += ./ui/OpgamForm.ui