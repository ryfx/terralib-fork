all: rasterSlicing

debug: rasterSlicing_dbg

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

rasterSlicing: terralib
	qmake -o makeRasterSlicing rasterSlicing.pro; make -f makeRasterSlicing

rasterSlicing_dbg: terralib_dbg
	qmake -o makeRasterSlicing.debug rasterSlicing.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeRasterSlicing.debug

clean:
	if \
		test -f makeRasterSlicing; \
	then \
		make -f makeRasterSlicing distclean; rm -f makeRasterSlicing; \
	fi
	
clean_debug:
	if \
		test -f makeRasterSlicing.debug; \
	then \
		make -f makeRasterSlicing.debug distclean; rm -f makeRasterSlicing.debug; \
	fi