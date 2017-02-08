#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = copyDatabase
include( ../base/base.pro )
win32:!win32-g++:QMAKE_LIBDIR += $$TERRALIBPATH/$$BUILDLOC/te_functions \
				$$TERRALIBPATH/$$BUILDLOC/te_utils
LIBS += -lte_functions -lte_utils
TRANSLATIONS = copyDatabase_pt.ts \
			copyDatabase_es.ts
INCLUDEPATH += ./include \
			$$TERRALIBPATH/src/terralib/utils
HEADERS +=  ./include/copydbplugin.h \
	./include/copydatabasePluginCode.h
SOURCES +=  ./src/copydbplugin.cpp \
	./src/copydatabasePluginCode.cpp
FORMS += ./ui/CopyDatabaseWindow.ui \
		./ui/DatabaseDialog.ui
