all: importGridData

debug: importGridData_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

importGridData: terralib
	qmake -o makeImportGridData importGridData.pro; make -f makeImportGridData

importGridData_dbg: terralib_dbg
	qmake -o makeImportGridData.debug importGridData.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeImportGridData.debug


clean:
	if \
		test -f makeImportGridData; \
	then \
		make -f makeImportGridData distclean; rm -f makeImportGridData; \
	fi
	
clean_debug:
	if \
		test -f makeImportGridData.debug; \
	then \
		make -f makeImportGridData.debug distclean; rm -f makeImportGridData.debug; \
	fi