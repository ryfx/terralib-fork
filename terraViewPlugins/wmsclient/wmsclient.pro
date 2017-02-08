#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
TARGET = wmsclient
win32:!win32-g++:TARGET = wmsclient
include( ../base/base.pro )
#TEMPLATE	= lib
#LANGUAGE	= C++
TRANSLATIONS = wmsclient_pt.ts \
			wmsclient_es.ts
#DEFINES += USE_XERCES  
unix:LIBS += -L../../../terraogc/lib
win32 {
	win32-g++:LIBS += -L../../../terraogc/lib
	else:QMAKE_LIBDIR += ../../../terraogc/lib
}
#debug {
#	LIBS += 
#	-lterraogccommon_d -lterraogcwmstheme_d -lterraogcwms_d \
#	    -lterraogcwmsclient_d -lterraogcows_d -lterraogcowstheme_d \
#	    -lterraogcxml_d -lterraogcxsd_d \
#	    -lterraogcse_d -lterraogcsld_d -lterraogcfilter_d -lterraogcowsclient_d \
#	    -lterraogcgml_d
#	unix:LIBS += -lterraogcxacml_d 
#}
#release {
#	LIBS += -lterraogccommon -lterraogcwmstheme -lterraogcwms \
#	    -lterraogcwmsclient -lterraogcows -lterraogcowstheme \
#	    -lterraogcxml -lterraogcxsd \
#	    -lterraogcse -lterraogcsld -lterraogcfilter -lterraogcowsclient \
#	    -lterraogcgml
#	unix:LIBS += -lterraogcxacml 
#}
unix {
#	debug:LIBS += -L../../../terraogc/Debug/linux-g++
#	release:LIBS += -L../../../terraogc/Release/linux-g++
	LIBS += -lgd -lxerces-c -lcurl \
		-lterraogccommon -lterraogcxml -lterraogcxsd \
		 -lterraogcows -lterraogcowsclient -lterraogcowstheme \
		-lterraogcse -lterraogcsld -lterraogcfilter  \
		-lterraogcgml -lterraogcxacml \
		 -lterraogcwms -lterraogcwmsclient -lterraogcwmstheme 
}
INCLUDEPATH += ./include \
			../../../terraogc/src \
			./images
HEADERS +=  ./include/wmsclientPlugin.h \
	./include/wmsclientPluginCode.h \
	./include/WMSClientPluginUtils.h \
	./include/WMSClientTerraViewUtils.h \
	./include/WMSConfigurer.h
SOURCES +=  ./src/wmsclientPlugin.cpp \
	./src/wmsclientPluginCode.cpp \
	./src/WMSClientPluginUtils.cpp \
	./src/WMSClientTerraViewUtils.cpp \
	./src/WMSConfigurer.cpp
FORMS += ./ui/WMSClientPluginMainForm.ui \
		./ui/WMSFeatureInfo.ui \
		./ui/WMSPopupWidget.ui \
		./ui/WMSPropertiesWindow.ui
IMAGES += ./images/wmsTheme.xpm
