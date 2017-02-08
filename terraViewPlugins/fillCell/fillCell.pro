#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = fillCell
include( ../base/base.pro )
TRANSLATIONS = fillCell_pt.ts \
		fillCell_es.ts
INCLUDEPATH += ./include
SOUT = linux-g++
win32 {
	win32-g++ {
		SOUT = win32-g++
	}
	else:QMAKE_LIBDIR += $$TERRALIBPATH/$$BUILDLOC/te_functions
}
LIBS += -L$$TERRALIBPATH/$$BUILDLOC/$$SOUT
LIBS += -lte_functions
HEADERS +=  ./include/fillcellplugin.h \
	./include/fillcellPluginCode.h \
	./include/FillCell.h \
	./include/FillCellDlg.h \
	./include/FillCellOp.h \
	./include/FillCellUtils.h
SOURCES +=  ./src/fillcellplugin.cpp \
	./src/fillcellPluginCode.cpp \
	./src/FillCell.cpp \
	./src/FillCellDlg.cpp \
	./src/FillCellOp.cpp \
	./src/FillCellUtils.cpp
FORMS += ./ui/FillCellUI.ui