all: importJPEG

debug: importJPEG_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

importJPEG: terralib
	qmake -o makeImportJPEG importJPEG.pro; make -f makeImportJPEG

importJPEG_dbg: terralib_dbg
	qmake -o makeImportJPEG.debug importJPEG.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeImportJPEG.debug

clean:
	if \
		test -f makeImportJPEG; \
	then \
		make -f makeImportJPEG distclean; rm -f makeImportJPEG; \
	fi
	
clean_debug:
	if \
		test -f makeImportJPEG.debug; \
	then \
		make -f makeImportJPEG.debug distclean; rm -f makeImportJPEG.debug; \
	fi