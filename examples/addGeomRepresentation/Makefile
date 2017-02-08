all: addGeomRepresentation

debug: addGeomRepresentation_dbg

terralib:
	cd ../../terralibx/terralib; make

terralib_dbg:
	cd ../../terralibx/terralib; make debug
	
addGeomRepresentation: terralib
	qmake -o makeAddGeomRepresentation addGeomRepresentation.pro; make -f makeAddGeomRepresentation

addGeomRepresentation_dbg: terralib_dbg
	qmake -o makeAddGeomRepresentation.debug addGeomRepresentation.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeAddGeomRepresentation.debug

clean:
	if \
		test -f makeAddGeomRepresentation; \
	then \
		make -f makeAddGeomRepresentation distclean; rm -f makeAddGeomRepresentation; \
	fi
	
clean_debug:
	if \
		test -f makeAddGeomRepresentation.debug; \
	then \
		make -f makeAddGeomRepresentation.debug distclean; rm -f makeAddGeomRepresentation.debug; \
	fi