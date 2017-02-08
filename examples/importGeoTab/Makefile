all: importGeoTab

debug: importGeoTab_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug
	
importGeoTab: terralib
	qmake -o makeImportGeoTab importGeoTab.pro; make -f makeImportGeoTab

importGeoTab_dbg: terralib_dbg
	qmake -o makeImportGeoTab.debug importGeoTab.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeImportGeoTab.debug

clean:
	if \
		test -f makeImportGeoTab; \
	then \
		make -f makeImportGeoTab distclean; rm -f makeImportGeoTab; \
	fi
	
clean_debug:
	if \
		test -f makeImportGeoTab.debug; \
	then \
		make -f makeImportGeoTab.debug distclean; rm -f makeImportGeoTab.debug; \
	fi