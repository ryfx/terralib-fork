all: createTheme

debug: createTheme_dbg

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

createTheme: terralib
	qmake -o makeCreateTheme createTheme.pro; make -f makeCreateTheme


createTheme_dbg: terralib_dbg
	qmake -o makeCreateTheme.debug createTheme.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeCreateTheme.debug

clean:
	if \
		test -f makeCreateTheme; \
	then \
		make -f makeCreateTheme distclean; rm -f makeCreateTheme; \
	fi
	
clean_debug:
	if \
		test -f makeCreateTheme.debug; \
	then \
		make -f makeCreateTheme.debug distclean; rm -f makeCreateTheme.debug; \
	fi