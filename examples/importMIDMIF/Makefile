all: importMIDMIF

debug: importMIDMIF_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

importMIDMIF: terralib
	qmake -o makeImportMIDMIF importMIDMIF.pro; make -f makeImportMIDMIF

importMIDMIF_dbg: terralib_dbg
	qmake -o makeImportMIDMIF.debug importMIDMIF.pro TE_PROJECT_TYPE=DEBUG; make -f makeImportMIDMIF.debug

clean:
	if \
		test -f makeImportMIDMIF; \
	then \
		make -f makeImportMIDMIF distclean; rm -f makeImportMIDMIF; \
	fi
	
clean_debug:
	if \
		test -f makeImportMIDMIF.debug; \
	then \
		make -f makeImportMIDMIF.debug distclean; rm -f makeImportMIDMIF.debug; \
	fi