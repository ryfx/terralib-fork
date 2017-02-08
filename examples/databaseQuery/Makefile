all: databaseQuery

debug: databaseQuery_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug

databaseQuery: terralib
	qmake -o makeDatabaseQuery databaseQuery.pro; make -f makeDatabaseQuery

databaseQuery_dbg: terralib_dbg
	qmake -o makeDatabaseQuery.debug databaseQuery.pro TE_PROJECT_TYPE=DEBUG;\
	make -f makeDatabaseQuery.debug

clean:
	if \
		test -f makeDatabaseQuery; \
	then \
		make -f makeDatabaseQuery distclean; rm -f makeDatabaseQuery; \
	fi
	
clean_debug:
	if \
		test -f makeDatabaseQuery.debug; \
	then \
		make -f makeDatabaseQuery.debug distclean; rm -f makeDatabaseQuery.debug; \
	fi