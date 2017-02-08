TEMPLATE = subdirs

SUBDIRS += addGeomRepresentation \
		./convertCoordinates \
		./copyLayer \
		./createDatabase \
		./createLayer \
		./createProximityMatrix \
		./createSTElementSet \
		./createSTElementSetFromLayer \
		./createSTElementSetFromShapeFile \
		./createSTElementSetFromTheme \
		./createTable \
		./createTheme \
		./createTIN \
		./databaseQuery \
		./databaseSQLQuery \
		./image_processing \
		./importCSV \
		./importDBF \
		./importGeoTab \
		./importGridData \
		./importJPEG \
		./importMIDMIF \
		./importShape \
		./mosaicTIFFImages \
		./proxMatrixAndSpatialStatistics \
		./querierFromLayer \
		./querierFromTheme \
		./querierGroupChronon \
		./querierGroupElement \
		./querierGroupSpatialRest \
		./querierWithSpatialRestBox \
		./querierWithSpatialRestGeometry \
		./rasterSlicing \
		./spatialQuery \
		./spatialQueryAndBuffer \
		./themeGrouping		
win32 {
	!win32-g++:SUBDIRS += ./createCoverage 
}