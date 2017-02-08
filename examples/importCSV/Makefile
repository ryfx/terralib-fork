all: importCSV

debug: importCSV_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

importCSV: terralib
	qmake -o makeImportCSV importCSV.pro; make -f makeImportCSV

importCSV_dbg: terralib_dbg
	qmake -o makeImportCSV.debug importCSV.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeImportCSV.debug


clean:
	if \
		test -f makeImportCSV; \
	then \
		make -f makeImportCSV distclean; rm -f makeImportCSV; \
	fi

clean_debug:
	if \
		test -f makeImportCSV.debug; \
	then \
		make -f makeImportCSV.debug distclean; rm -f makeImportCSV.debug; \
	fi