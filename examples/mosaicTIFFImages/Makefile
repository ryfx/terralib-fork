all: mosaicTIFFImages

debug: mosaicTIFFImages_dbg

tiff:
	cd ../../terralibx/tiff; make

terralib:
	cd ../../terralibx/terralib; make
	
terralib_dbg:
	cd ../../terralibx/terralib; make debug

mosaicTIFFImages:  tiff terralib 
	qmake -o makeMosaicTIFFImages mosaicTIFFImages.pro; make -f makeMosaicTIFFImages

mosaicTIFFImages_dbg:  tiff terralib_dbg 
	qmake -o makeMosaicTIFFImages.debug mosaicTIFFImages.pro TE_PROJECT_TYPE=DEBUG; \
	make -f makeMosaicTIFFImages.debug

clean:
	if \
		test -f makeMosaicTIFFImages; \
	then \
		make -f makeMosaicTIFFImages distclean; rm -f makeMosaicTIFFImages; \
	fi
	
clean_debug:
	if \
		test -f makeMosaicTIFFImages.debug; \
	then \
		make -f makeMosaicTIFFImagesdebug distclean; rm -f makeMosaicTIFFImages.debug; \
	fi