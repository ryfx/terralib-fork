all: querierFromLayer

debug: querierFromLayer_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug
	
querierFromLayer: terralib
	qmake -o makeQuerierFromLayer querierFromLayer.pro; make -f makeQuerierFromLayer

querierFromLayer_dbg: terralib_dbg
	qmake -o makeQuerierFromLayer.debug querierFromLayer.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeQuerierFromLayer.debug

clean:
	if \
		test -f makeQuerierFromLayer; \
	then \
		make -f makeQuerierFromLayer distclean; rm -f makeQuerierFromLayer; \
	fi
	
clean_debug:
	if \
		test -f makeQuerierFromLayer.debug; \
	then \
		make -f makeQuerierFromLayer.debug distclean; rm -f makeQuerierFromLayer.debug; \
	fi