all: copyLayer

debug: copyLayer_dbg

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

copyLayer: terralib
	qmake -o makeCopyLayer copyLayer.pro; make -f makeCopyLayer

copyLayer_dbg: terralib_dbg
	qmake -o makeCopyLayer.debug copyLayer.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeCopyLayer.debug;	

clean:
	if \
		test -f makeCopyLayer; \
	then \
		make -f makeCopyLayer distclean; rm -f makeCopyLayer; \
	fi
	
clean_debug:
	if \
		test -f makeCopyLayer.debug; \
	then \
		make -f makeCopyLayer.debug distclean; rm -f makeCopyLayer.debug; \
	fi	