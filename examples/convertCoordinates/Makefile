all: convertCoordinates

debug: convertCoordinates_dbg

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

convertCoordinates: terralib
	qmake -o makeConvertCoordinates convertCoordinates.pro; \
	make -f makeConvertCoordinates

convertCoordinates_dbg: terralib_dbg
	qmake -o makeConvertCoordinates.debug convertCoordinates.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeConvertCoordinates.debug

clean:
	if \
		test -f makeConvertCoordinates; \
	then \
		make -f makeConvertCoordinates distclean; rm -f makeConvertCoordinates; \
	fi
	
	clean_debug:
	if \
		test -f makeConvertCoordinates.debug; \
	then \
		make -f makeConvertCoordinates.debug distclean; rm -f makeConvertCoordinates.debug; \
	fi