TERRALIBPATH = ../../..

include( $$TERRALIBPATH/terraViewPlugins/base/base.pro )

INCLUDEPATH += $$TERRALIBPATH/src/terralib/utils

LIBS += -lte_spl

win32 {
	debug:DLLDESTDIR = $$TERRALIBPATH/terraView/windows/plugins
	!win32-g++:release:DLLDESTDIR = $$TERRALIBPATH/Release/plugins
}
