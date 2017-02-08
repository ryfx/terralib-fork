all: querierGroupElement

terralib:
	cd ../../terralibx/terralib; make

querierGroupElement: terralib
	qmake -o makeQuerierGroupElement querierGroupElement.pro; make -f makeQuerierGroupElement

clean:
	if \
		test -f makeQuerierGroupElement; \
	then \
		make -f makeQuerierGroupElement distclean; rm -f makeQuerierGroupElement; \
	fi