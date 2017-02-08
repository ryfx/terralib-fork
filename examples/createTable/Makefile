all: createTable

debug: createTable_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

createTable:  terralib 
	qmake -o makeCreateTable createTable.pro; make -f makeCreateTable

createTable_dbg: terralib_dbg
	qmake -o makeCreateTable.debug createTable.pro TE_PROJECT_TYPE=DEBUG; make -f makeCreateTable.debug

clean:
	if \
		test -f makeCreateTable; \
	then \
		make -f makeCreateTable distclean; rm -f makeCreateTable; \
	fi
	
	clean_debug:
	if \
		test -f makeCreateTable.debug; \
	then \
		make -f makeCreateTable.debug distclean; rm -f makeCreateTable.debug; \
	fi
