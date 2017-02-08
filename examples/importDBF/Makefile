all: importDBF

debug: importDBF_dbg

shapelib:
	cd ../../terralibx/shapelib; make
	
shapelib_dbg:
	cd ../../terralibx/shapelib; make debug

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

importDBF: shapelib terralib 
	qmake -o makeImportDBF importDBF.pro; make -f makeImportDBF

importDBF_dbg: shapelib_dbg terralib_dbg 
	qmake -o makeImportDBF.debug importDBF.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeImportDBF.debug


clean:
	if \
		test -f makeImportDBF; \
	then \
		make -f makeImportDBF distclean; rm -f makeImportDBF; \
	fi
	
clean_debug:
	if \
		test -f makeImportDBF.debug; \
	then \
		make -f makeImportDBF.debug distclean; rm -f makeImportDBF.debug; \
	fi