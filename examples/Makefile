all: release

debug: tlib_dbg build_dbg

tlib_dbg:
	cd ../build/qt; qmake utilities.pro TEDRIVERS="HAS_MYSQL HAS_SHP" MAKEFILE=Makefile.Debug TE_PROJECT_TYPE=DEBUG; make -f Makefile.Debug;
	
build_dbg:
	qmake MAKEFILE=Makefile.Debug TE_PROJECT_TYPE=DEBUG; make -f Makefile.Debug