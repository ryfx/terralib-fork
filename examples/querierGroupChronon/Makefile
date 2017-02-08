all: querierGroupChronon

terralib:
	cd ../../terralibx/terralib; make


querierGroupChronon: terralib 
	qmake -o makeQuerierGroupChronon querierGroupChronon.pro; make -f makeQuerierGroupChronon

clean:
	if \
		test -f makeQuerierGroupChronon; \
	then \
		make -f makeQuerierGroupChronon distclean; rm -f makeQuerierGroupChronon; \
	fi