#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = KmlPlugin
include( ../base/base.pro )
#TEMPLATE	=app
LANGUAGE	= C++
TRANSLATIONS = kmlplugin_pt.ts \
		kmlplugin_sp.ts
	
unix {
	LIBS += -L../../../terraogc/lib \
			-lxerces-c -lcurl
}
win32 {
	win32-g++:LIBS += -L../../../terraogc/lib -L../../../thirdparty/libcurl-7.15.5-win32-msvc		
	else:QMAKE_LIBDIR += ../../../terraogc/lib ../../../thirdparty/libcurl-7.15.5-win32-msvc
}	
debug {
	LIBS += -lterraogccommon_d -lterraogcxml_d	
}
release {
	LIBS += -lterraogccommon -lterraogcxml	
}

#Path definitions
#----------------------------------
INCLUDEFILESPATH = ./include
SRCPATH = ./src
#-----------------------------------

INCLUDEPATH	+= ./include \
	../../../terraogc/src \	
	../../src/terralib/kernel \
	../../terraViewPlugins/base \
	../../terraView \
	../../terraView/ui \
	../../ui/terraView \
	../../ui/appUtils \
	../../src/libspl \
	../../src/terralib/functions \
	../../src/terralib/drivers/qt \
	../../src/terralib/drivers/spl \
	../../src/appUtils \
	../../ui \
	$(QTDIR)/include \
	./src \
	./src/ui \
	./src/moc \
	$(QTDIR)/mkspecs/win32-msvc.net \
	../../../terraogc/src \
	../../../terraogc/src/xml \
	../../../thirdparty/libcurl-7.15.5-win32-msvc/include \
	./ui/images
	
SOURCES	+= $$SRCPATH/kmlplugin.cpp \
	$$SRCPATH/kmlpluginPluginCode.cpp \
	$$SRCPATH/kmldocument.cpp \
	$$SRCPATH/kmlfeature.cpp \
	$$SRCPATH/kmlfactories.cpp \
	$$SRCPATH/kmlfactoriesregister.cpp \
	$$SRCPATH/kmlfolder.cpp \
	$$SRCPATH/kmlmanager.cpp \
	$$SRCPATH/kmlobject.cpp \
	$$SRCPATH/kmlplacemark.cpp \
	$$SRCPATH/kmlstyle.cpp \
	$$SRCPATH/kmlstylemap.cpp \
	$$SRCPATH/kmlstylesmanager.cpp \
	$$SRCPATH/kmlsubstyle.cpp \
	$$SRCPATH/kmlpolygonstyle.cpp \
	$$SRCPATH/kmllinestyle.cpp \
	$$SRCPATH/kmlpointstyle.cpp \
	$$SRCPATH/kmlreader.cpp \
	$$SRCPATH/kmlimporter.cpp \
	$$SRCPATH/kmlwriter.cpp \
	$$SRCPATH/KmlInformationBalloon.cpp \
	$$SRCPATH/kmlchecklistitem.cpp \
	$$SRCPATH/kmlexporter.cpp \
	$$SRCPATH/KmlIconRequest.cpp \
	$$SRCPATH/kmlinterpreter.cpp \
	$$SRCPATH/TeKmlTheme.cpp \
	$$SRCPATH/KmlPluginUtils.cpp 
HEADERS	+= $$INCLUDEFILESPATH/kmlplugin.h \
	$$INCLUDEFILESPATH/kmlpluginPluginCode.h \
	$$INCLUDEFILESPATH/kmlabstractvisitor.h \
	$$INCLUDEFILESPATH/kmldefines.h \
	$$INCLUDEFILESPATH/kmldocument.h \
	$$INCLUDEFILESPATH/kmlfactories.h \
	$$INCLUDEFILESPATH/kmlfeature.h \
	$$INCLUDEFILESPATH/kmlfolder.h \
	$$INCLUDEFILESPATH/kmlmanager.h \
	$$INCLUDEFILESPATH/kmlobject.h \
	$$INCLUDEFILESPATH/kmlobjectfactory.h \
	$$INCLUDEFILESPATH/kmlplacemark.h \
	$$INCLUDEFILESPATH/kmlstructureitem.h \
	$$INCLUDEFILESPATH/kmlstyle.h \
	$$INCLUDEFILESPATH/kmlsubstyle.h \
	$$INCLUDEFILESPATH/kmlpolygonstyle.h \
	$$INCLUDEFILESPATH/kmllinestyle.h \
	$$INCLUDEFILESPATH/kmlpointstyle.h \
	$$INCLUDEFILESPATH/kmlstylemap.h \
	$$INCLUDEFILESPATH/kmlstylesmanager.h \
	$$INCLUDEFILESPATH/visitreceiver.h \
	$$INCLUDEFILESPATH/KmlInformationBalloon.h \
	$$INCLUDEFILESPATH/kmlchecklistitem.h \
	$$INCLUDEFILESPATH/kmlexporter.h \
	$$INCLUDEFILESPATH/KmlIconRequest.h \
	$$INCLUDEFILESPATH/kmlimporter.h \
	$$INCLUDEFILESPATH/kmlinterpreter.h \
	$$INCLUDEFILESPATH/kmlreader.h \
	$$INCLUDEFILESPATH/kmlwriter.h \
	$$INCLUDEFILESPATH/TeKmlTheme.h \
	$$INCLUDEFILESPATH/KmlPluginUtils.h 
FORMS	= ui/ImportKmlWindow.ui \
	ui/ExportKmlWindow.ui \
	ui/KmlThemeWindow.ui \
	ui/KmlPopupWidget.ui	
IMAGES	= ./ui/images/KmlTheme.bmp \
	./ui/images/ImportKml.bmp \
	./ui/images/ExportKml.bmp \
	./ui/images/KmlThemeInvalid.bmp \
	./ui/images/Balloon.png \
	./ui/images/ExitButton.png \
	./ui/images/Point.png