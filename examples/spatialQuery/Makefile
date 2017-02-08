all: spatialQuery

debug: spatialQuery_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug
	
spatialQuery: terralib 
	qmake -o makeSpatialQuery spatialQuery.pro; make -f makeSpatialQuery

spatialQuery_dbg: terralib_dbg 
	qmake -o makeSpatialQuery.debug spatialQuery.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeSpatialQuery.debug


clean:
	if \
		test -f makeSpatialQuery; \
	then \
		make -f makeSpatialQuery distclean; rm -f makeSpatialQuery; \
	fi
	
clean_debug:
	if \
		test -f makeSpatialQuery.debug; \
	then \
		make -f makeSpatialQuery.debug distclean; rm -f makeSpatialQuery.debug; \
	fi	