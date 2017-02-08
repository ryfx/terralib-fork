all: querierFromTheme

debug: querierFromTheme_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug
	
querierFromTheme:  terralib 
	qmake -o makeQuerierFromTheme querierFromTheme.pro; make -f makeQuerierFromTheme

querierFromTheme_dbg:  terralib_dbg 
	qmake -o makeQuerierFromTheme.debug querierFromTheme.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeQuerierFromTheme.debug

clean:
	if \
		test -f makeQuerierFromTheme; \
	then \
		make -f makeQuerierFromTheme distclean; rm -f makeQuerierFromTheme; \
	fi
	
clean_debug:
	if \
		test -f makeQuerierFromTheme.debug; \
	then \
		make -f makeQuerierFromTheme.debug distclean; rm -f makeQuerierFromTheme.debug; \
	fi