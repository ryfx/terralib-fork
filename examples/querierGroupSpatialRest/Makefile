all: querierGroupSpatialRest

terralib:
	cd ../../terralibx/terralib; make

querierGroupSpatialRest: terralib
	qmake -o makeQuerierGroupSpatialRest querierGroupSpatialRest.pro; make -f makeQuerierGroupSpatialRest

clean:
	if \
		test -f makeQuerierGroupSpatialRest; \
	then \
		make -f makeQuerierGroupSpatialRest distclean; rm -f makeQuerierGroupSpatialRest; \
	fi