#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = Score
include( ../base.pri )
TRANSLATIONS = score_pt.ts \
		score_sp.ts
INCLUDEPATH += ./include
HEADERS +=  ./include/scoreplugin.h \
	./include/scorePluginCode.h \
	./include/normal01.h \
	./include/Score.h \
	./include/ScorePluginWindow.h
SOURCES +=  ./src/scoreplugin.cpp \
	./src/scorePluginCode.cpp \
	./src/Score.cpp \
	./src/ScorePluginWindow.cpp
FORMS += ./ui/ScoreForm.ui