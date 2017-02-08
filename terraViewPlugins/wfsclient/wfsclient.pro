#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------
#Path definitions
# ------------------------------------
SPATH = ./src
INPATH = ./include
FPATH = ./ui
IMPATH = ./images
# ------------------------------------
# Project definitions
# ------------------------------------
TARGET = wfsclient
include( ../base/base.pro )
# ------------------------------------
# TerraOG Path definitions
# ------------------------------------
TEOGCPATH = $${TERRALIBPATH}/../terraogc
# ------------------------------------
TRANSLATIONS = wfsclient_pt.ts \
		wfsclient_sp.ts
INCLUDEPATH += $${INPATH} \
			$${TEOGCPATH}/src
win32 {
	win32-g++:LIBS += -L$${TEOGCPATH}/lib
	else:QMAKE_LIBDIR += $${TEOGCPATH}/lib
}
unix {
#	debug:LIBS += -L$${TEOGCPATH}/Debug/linux-g++
#	release:LIBS += -L$${TEOGCPATH}/Release/linux-g++
	LIBS += -L$${TEOGCPATH}/lib \
		-lgd -lxerces-c -lcurl
}

debug:LIBS += -lterraogccommon_d -lterraogcxml_d -lterraogcows_d -lterraogcowsclient_d -lterraogcowstheme_d -lterraogcwfs_d -lterraogcwfsclient_d \
			-lterraogcxsd_d -lterraogcwfstheme_d -lterraogcfilter_d -lterraogcxacml_d

release:LIBS += -lterraogccommon -lterraogcxml -lterraogcows -lterraogcowsclient -lterraogcowstheme -lterraogcwfs -lterraogcwfsclient -lterraogcwfstheme \
				-lterraogcxsd -lterraogcfilter -lterraogcxacml

HEADERS +=  $${INPATH}/wfsplugin.h \
	$${INPATH}/wfsclientPluginCode.h \
	$${INPATH}/wfsPluginUtils.h \
	$${INPATH}/wfsClientTerraViewUtils.h \
	$${INPATH}/wfsThemeMenu.h
SOURCES +=  $${SPATH}/wfsplugin.cpp \
	$${SPATH}/wfsclientPluginCode.cpp \
	$${SPATH}/wfsPluginUtils.cpp \
	$${SPATH}/wfsClientTerraViewUtils.cpp \
	$${SPATH}/wfsThemeMenu.cpp
FORMS += $${FPATH}/wfsconfig.ui \
		$${FPATH}/wfsattr.ui \
		$${FPATH}/wfsimport.ui
IMAGES += $${IMPATH}/wmsTheme.xpm
win32 {
	QMAKE_POST_LINK = $${QMAKE_COPY} ..\..\..\terraogc\bin\*_d.dll .\Debug\plugins && \
					$${QMAKE_COPY} ..\..\..\thirdparty\xerces-c-src_2_8_0\Build\Win32\VC8\Debug\*D.dll .\Debug\plugins && \
					$${QMAKE_COPY} ..\..\..\thirdparty\libcurl-7.15.5-win32-msvc\*.dll .\Debug\plugins
}