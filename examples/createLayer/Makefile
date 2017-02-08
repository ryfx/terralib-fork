all: createLayer

debug: createLayer_dbg

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

createLayer: terralib
	qmake -o makeCreateLayer createLayer.pro; make -f makeCreateLayer

createLayer_dbg: terralib_dbg
	qmake -o makeCreateLayer.debug createLayer.pro TE_PROJECT_TYPE=DEBUG; make -f makeCreateLayer.debug
	
clean:
	if \
		test -f makeCreateLayer; \
	then \
		make -f makeCreateLayer distclean; rm -f makeCreateLayer; \
	fi
	
clean_debug:
	if \
		test -f makeCreateLayer.debug; \
	then \
		make -f makeCreateLayer.debug distclean; rm -f makeCreateLayer.debug; \
	fi

