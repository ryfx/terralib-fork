all: proxMatrixAndSpatialStatistics

debug: proxMatrixAndSpatialStatistics_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug
	
stat:
	cd ../../terralibx/stat; make

proxMatrixAndSpatialStatistics: terralib stat
	qmake -o makeProxMatrixAndSpatialStatistics proxMatrixAndSpatialStatistics.pro; make -f makeProxMatrixAndSpatialStatistics

proxMatrixAndSpatialStatistics_dbg: terralib_dbg stat
	qmake -o makeProxMatrixAndSpatialStatistics.debug proxMatrixAndSpatialStatistics.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeProxMatrixAndSpatialStatistics.debug

clean:
	if \
		test -f makeProxMatrixAndSpatialStatistics; \
	then \
		make -f makeProxMatrixAndSpatialStatistics distclean; rm -f makeProxMatrixAndSpatialStatistics; \
	fi
	
clean:
	if \
		test -f makeProxMatrixAndSpatialStatistics.debug; \
	then \
		make -f makeProxMatrixAndSpatialStatistics.debug distclean; rm -f makeProxMatrixAndSpatialStatistics.debug; \
	fi