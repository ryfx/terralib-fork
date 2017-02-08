all: createDatabase

debug: createDB_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

createDatabase: terralib
	qmake -o makeCreateDatabase createDatabase.pro; make -f makeCreateDatabase

createDB_dbg: terralib_dbg
	qmake -o makeCreateDatabase.debug createDatabase.pro TE_PROJECT_TYPE=DEBUG; make -f makeCreateDatabase.debug

clean:
	if \
		test -f makeCreateDatabase; \
	then \
		make -f makeCreateDatabase distclean; rm -f makeCreateDatabase; \
	fi

clean_debug:
	if \
		test -f makeCreateDatabase.debug; \
	then \
		make -f makeCreateDatabase.debug distclean; rm -f makeCreateDatabase.debug; \
	fi