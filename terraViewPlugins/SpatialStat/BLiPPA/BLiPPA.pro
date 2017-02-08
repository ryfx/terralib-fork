#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = BLiPPA
include( ../base.pri )
TRANSLATIONS = blippa_pt.ts \
		blippa_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/blippaplugin.h \
	./include/blippaPluginCode.h \
	./include/algoritmos.h \
	./include/BLiPPAPluginWindow.h \
	./include/CImg.h \
	./include/LoadParams.h \
	./include/sv.h
SOURCES +=  ./src/blippaplugin.cpp \
	./src/blippaPluginCode.cpp \
	./src/algoritmos.cpp \
	./src/BLiPPAPluginWindow.cpp \
	./src/LoadParams.cpp \
	./src/sv.cpp
FORMS += ./ui/BLiPPAForm.ui