all: databaseSQLQuery

debug: databaseSQLQuery_dbg

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

databaseSQLQuery: terralib
	qmake -o makeDatabaseSQLQuery databaseSQLQuery.pro; make -f makeDatabaseSQLQuery

databaseSQLQuery_dbg: terralib_dbg
	qmake -o makeDatabaseSQLQuery.debug databaseSQLQuery.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeDatabaseSQLQuery.debug

clean:
	if \
		test -f makeDatabaseSQLQuery; \
	then \
		make -f makeDatabaseSQLQuery distclean; rm -f makeDatabaseSQLQuery; \
	fi
	
clean_debug:
	if \
		test -f makeDatabaseSQLQuery.debug; \
	then \
		make -f makeDatabaseSQLQuery.debug distclean; rm -f makeDatabaseSQLQuery.debug; \
	fi