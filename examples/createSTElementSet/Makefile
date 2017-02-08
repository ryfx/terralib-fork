all: createSTElementSet

debug: createSTElementSet_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

createSTElementSet: terralib
	qmake -o makeCreateSTElementSet createSTElementSet.pro; make -f makeCreateSTElementSet

createSTElementSet_dbg: terralib_dbg
	qmake -o makeCreateSTElementSet.debug createSTElementSet.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeCreateSTElementSet.debug

clean:
	if \
		test -f makeCreateSTElementSet; \
	then \
		make -f makeCreateSTElementSet distclean; rm -f makeCreateSTElementSet; \
	fi
	
clean_debug:
	if \
		test -f makeCreateSTElementSet.debug; \
	then \
		make -f makeCreateSTElementSet.debug distclean; rm -f makeCreateSTElementSet.debug; \
	fi