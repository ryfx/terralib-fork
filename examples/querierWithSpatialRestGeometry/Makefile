all: querierWithSpatialRestGeometry

terralib:
	cd ../../terralibx/terralib; make

querierWithSpatialRestGeometry: terralib
	qmake -o makeQuerierWithSpatialRestGeometry querierWithSpatialRestGeometry.pro; make -f makeQuerierWithSpatialRestGeometry

clean:
	if \
		test -f makeQuerierWithSpatialRestGeometry; \
	then \
		make -f makeQuerierWithSpatialRestGeometry distclean; rm -f makeQuerierWithSpatialRestGeometry; \
	fi