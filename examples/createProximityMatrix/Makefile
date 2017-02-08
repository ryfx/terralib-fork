all: createProximityMatrix

terralib:
	cd ../../terralibx/terralib; make

createProximityMatrix: terralib 
	qmake -o makeCreateProximityMatrix createProximityMatrix.pro; make -f makeCreateProximityMatrix

clean:
	if \
		test -f makeCreateProximityMatrix; \
	then \
		make -f makeCreateProximityMatrix distclean; rm -f makeCreateProximityMatrix; \
	fi