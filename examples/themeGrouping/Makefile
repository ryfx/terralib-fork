all: themeGrouping

debug: themeGrouping_dbg

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

themeGrouping:  terralib 
	qmake -o makeThemeGrouping themeGrouping.pro; make -f makeThemeGrouping

themeGrouping_dbg:  terralib_dbg 
	qmake -o makeThemeGrouping.debug themeGrouping.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeThemeGrouping.debug

clean:
	if \
		test -f makeThemeGrouping; \
	then \
		make -f makeThemeGrouping distclean; rm -f makeThemeGrouping; \
	fi
	
clean_debug:
	if \
		test -f makeThemeGrouping.debug; \
	then \
		make -f makeThemeGrouping.debug distclean; rm -f makeThemeGrouping.debug; \
	fi