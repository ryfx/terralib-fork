# ---------------------------------------------------------------------------
# Global variables

isEmpty(TERRALIBPATH) {
  TERRALIBPATH = ../..
}
isEmpty(TVIEWPLGPATH) {
  TVIEWPLGPATH = ..
}
isEmpty(TVIEWPATH) {
  TVIEWPATH = $$TERRALIBPATH/terraView
}

TEMPLATE = lib
CONFIG += dll
include ($$TERRALIBPATH/build/qt/config.pri)

CONFIG += qt

# ---------------------------------------------------------------------------
# Global project definitions
UI_DIR = src/ui
MOC_DIR = src/moc
# ---------------------------------------------------------------------------
# includes/sources

INCLUDEPATH += \
  . \
  $$TERRALIBPATH/terraViewPlugins/base \
  $$TERRALIBPATH/terraView \
  $$TERRALIBPATH/terraView/ui \
  $$TERRALIBPATH/ui/terraView \
  $$TERRALIBPATH/ui/appUtils \
  $$TERRALIBPATH/src/libspl \
  $$TERRALIBPATH/src/terralib/kernel \
  $$TERRALIBPATH/src/terralib/functions \
  $$TERRALIBPATH/src/terralib/drivers/qt \
  $$TERRALIBPATH/src/terralib/drivers/spl \
  $$TERRALIBPATH/src/appUtils \
  $$TERRALIBPATH/ui

# ---------------------------------------------------------------------------  
# Unix Plataform specific definitions

unix {
  
  INCLUDEPATH += $$TERRALIBPATH/terraView/linux/ui
    
  LIBS += -L$$TERRALIBPATH/terralibx/$$BUILDLOC \
		-lterralib -lte_apputils -lte_spl -lterraViewCore -lspl
}

# ---------------------------------------------------------------------------  
# win32 Plataform specific definitions

win32 {
  win32-g++:LIBS += -L$$TERRALIBPATH/terralibx/$$BUILDLOC \
			-lspl
  else {
	QMAKE_LIBDIR += $$TERRALIBPATH/$$BUILDLOC/terralib \
				$$TERRALIBPATH/$$BUILDLOC/te_apputils \
				$$TERRALIBPATH/$$BUILDLOC/libspl \
				$$TERRALIBPATH/$$BUILDLOC/te_spl \
				$$TERRALIBPATH/$$BUILDLOC/terraViewCore 
	LIBS += -llibspl
  }
  LIBS += -lterralib -lte_apputils -lterraViewCore -lte_spl
}
unix:OSPATH = linux-g++
win32 {
	OSPATH = win32-msvc
	win32-g++:OSPATH = win32-g++
}
DESTDIR = ../$$BUILDLOC/$$OSPATH
OBJECTS_DIR = $$DESTDIR/obj/$$TARGET
win32 {
	!win32-g++ {
		QMAKE_CXXFLAGS_MT_DLLDBG -= -Fd$$OBJECTS_DIR/
		QMAKE_CXXFLAGS_MT_DLLDBG += -Fd$$OBJECTS_DIR/
		QMAKE_CXXFLAGS_MT_DLL -= -Fd$$OBJECTS_DIR/
		QMAKE_CXXFLAGS_MT_DLL += -Fd$$OBJECTS_DIR/
	}
}

unix:OSPATH = linux-g++
win32-g++:OSPATH = win32-g++

PLGPATH = $$TERRALIBPATH/$$BUILDLOC/$$OSPATH

trans.path = $$PLGPATH/plugins
trans.files = *.qm

target.path = $$trans.path

INSTALLS += trans target