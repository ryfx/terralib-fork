all: importShape

debug: importShape_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug
	
importShape: terralib
	qmake -o makeImportShape importShape.pro; \
	make -f makeImportShape;
	
importShape_dbg: terralib_dbg
	qmake -o makeImportShape.debug importShape.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeImportShape.debug;	

clean:
	if \
		test -f makeImportShape; \
	then \
		make -f makeImportShape distclean; rm -f makeImportShape; \
	fi
	
clean_debug:
	if \
		test -f makeImportShape.debug; \
	then \
		make -f makeImportShape.debug distclean; rm -f makeImportShape.debug; \
	fi
	
