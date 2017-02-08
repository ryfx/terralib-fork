all: querierWithSpatialRestBox

terralib:
	cd ../../terralibx/terralib; make

querierWithSpatialRestBox: terralib
	qmake -o makeQuerierWithSpatialRestBox querierWithSpatialRestBox.pro; make -f makeQuerierWithSpatialRestBox

clean:
	if \
		test -f makeQuerierWithSpatialRestBox; \
	then \
		make -f makeQuerierWithSpatialRestBox distclean; rm -f makeQuerierWithSpatialRestBox; \
	fi