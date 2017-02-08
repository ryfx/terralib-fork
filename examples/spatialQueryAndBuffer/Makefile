all: spatialQueryAndBuffer

debug: spatialQueryAndBuffer_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

spatialQueryAndBuffer: terralib 
	qmake -o makeSpatialQueryAndBuffer spatialQueryAndBuffer.pro; make -f makeSpatialQueryAndBuffer

spatialQueryAndBuffer_dbg: terralib_dbg 
	qmake -o makeSpatialQueryAndBuffer.debug spatialQueryAndBuffer.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeSpatialQueryAndBuffer.debug

clean:
	if \
		test -f makeSpatialQueryAndBuffer; \
	then \
		make -f makeSpatialQueryAndBuffer distclean; rm -f makeSpatialQueryAndBuffer; \
	fi
	
clean_dbg:
	if \
		test -f makeSpatialQueryAndBuffer.debug; \
	then \
		make -f makeSpatialQueryAndBuffer.debug distclean; rm -f makeSpatialQueryAndBuffer.debug; \
	fi