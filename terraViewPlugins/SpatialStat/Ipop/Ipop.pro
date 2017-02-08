#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Ipop
include( ../base.pri )
TRANSLATIONS = ipop_pt.ts \
		ipop_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/ipopplugin.h \
	./include/ipopPluginCode.h \
	./include/ip.h \
	./include/IpopPluginWindow.h 
SOURCES +=  ./src/ipopplugin.cpp \
	./src/ipopPluginCode.cpp \
	./src/ip.cpp \
	./src/IpopPluginWindow.cpp 
FORMS += ./ui/IpopForm.ui