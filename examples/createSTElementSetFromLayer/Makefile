all: createSTElementSetFromLayer

debug: createSTElementSetFromLayer_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

createSTElementSetFromLayer: terralib
	qmake -o makeCreateSTElementSetFromLayer createSTElementSetFromLayer.pro; make -f makeCreateSTElementSetFromLayer

createSTElementSetFromLayer_dbg: terralib_dbg
	qmake -o makeCreateSTElementSetFromLayer.debug createSTElementSetFromLayer.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeCreateSTElementSetFromLayer.debug

clean:
	if \
		test -f makeCreateSTElementSetFromLayer; \
	then \
		make -f makeCreateSTElementSetFromLayer distclean; rm -f makeCreateSTElementSetFromLayer; \
	fi
	
clean_debug:
	if \
		test -f makeCreateSTElementSetFromLayer.debug; \
	then \
		make -f makeCreateSTElementSetFromLayer.debug distclean; rm -f makeCreateSTElementSetFromLayer.debug; \
	fi