all: createTIN

terralib:
	cd ../../terralibx/terralib; make

convertCoordinates: terralib
	qmake -o createTIN createTIN.pro; make -f createTIN

clean:
	if \
		test -f createTIN; \
	then \
		make -f createTIN distclean; rm -f createTIN; \
	fi