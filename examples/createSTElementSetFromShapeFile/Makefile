all: createSTElementSetFromShapeFile

shapelib:
	cd ../../terralibx/shapelib; make

terralib:
	cd ../../terralibx/terralib; make

createSTElementSetFromShapeFile: shapelib terralib
	qmake -o makeCreateSTElementSetFromShapeFile createSTElementSetFromShapeFile.pro; make -f makeCreateSTElementSetFromShapeFile

clean:
	if \
		test -f makeCreateSTElementSetFromShapeFile; \
	then \
		make -f makeCreateSTElementSetFromShapeFile distclean; rm -f makeCreateSTElementSetFromShapeFile; \
	fi