#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Tango
include( ../base.pri )
TRANSLATIONS = tango_pt.ts \
		tango_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/tangoplugin.h \
	./include/tangoPluginCode.h \
	./include/controlw.h \
	./include/dcdflib.h \
	./include/include.h \
	./include/myexcept.h \
	./include/newmat.h \
	./include/newmatap.h \
	./include/newmatrc.h \
	./include/precisio.h \
	./include/ranker.h \
	./include/Tango.h \
	./include/TangoPluginWindow.h
SOURCES +=  ./src/tangoplugin.cpp \
	./src/tangoPluginCode.cpp \
	./src/bandmat.cpp \
	./src/dcdflib.cpp \
	./src/myexcept.cpp \
	./src/newmat1.cpp \
	./src/newmat2.cpp \
	./src/newmat3.cpp \
	./src/newmat4.cpp \
	./src/newmat5.cpp \
	./src/newmat6.cpp \
	./src/newmat7.cpp \
	./src/newmat8.cpp \
	./src/newmatex.cpp \
	./src/submat.cpp \
	./src/Tango.cpp \
	./src/TangoPluginWindow.cpp
FORMS += ./ui/TangoForm.ui