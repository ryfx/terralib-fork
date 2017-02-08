SUBDIRS = . image_processing
DIST_SUBDIRS = $(SUBDIRS)

EXTRA_DIST = \
	data/BairroBH.MID \
	data/BairroBH.MIF \
	data/BairrosPoA.dbf \
	data/BairrosPoA.shp \
	data/BairrosPoA.shx \
	data/BairrosRecife2.csv \
	data/BairrosRecife.mid \
	data/BairrosRecife.mif \
	data/BrasiliaPol.dbf \
	data/BrasiliaPol.shp \
	data/BrasiliaPol.shx \
	data/Brasilia_RGB.tif \
	data/Cadastro_Escolas.geo \
	data/Cadastro_Escolas.tab \
	data/Distritos.mid \
	data/Distritos.mif \
	data/elevation.raw \
	data/EstadosBrasil.dbf \
	data/EstadosBrasil.shp \
	data/EstadosBrasil.shx \
	data/Mapa_Distritos_SP.dbf \
	data/Mapa_Distritos_SP.shp \
	data/Mapa_Distritos_SP.shx \
	data/Mapa_Drenagem_SP.dbf \
	data/Mapa_Drenagem_SP.shp \
	data/Mapa_Drenagem_SP.shx \
	data/Mapa_Industrias_SP.dbf \
	data/Mapa_Industrias_SP.shp \
	data/Mapa_Industrias_SP.shx \
	data/nat1.tif \
	data/nat2.tif \
	data/OcorrenciasBH.csv \
	data/OcorrenciasPoA.dbf \
	data/OcorrenciasPoA.shp \
	data/OcorrenciasPoA.shx \
	data/sampa.jpg \
	data/samples.spr \
	data/SOCEC.dbf \
	data/sp_589x703.raw
	
bin_PROGRAMS =

#########################################################################
# addGeomRepresentation example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_addGeomRepresentation
endif

___bin_terralib_examples_addGeomRepresentation_SOURCES = \
	addGeomRepresentation/addGeomRepresentation.cpp 

#########################################################################
# connectionPool example

if TE_BUILD_POSTGIS_BASED_EXAMPLE
if TE_BUILD_OPENMP_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_connectionPool
endif
endif

___bin_terralib_examples_connectionPool_SOURCES = \
	connectionPool/main.cpp 
	
#########################################################################
# convertCoordinates example

if TE_BUILD_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_convertCoordinates
endif

___bin_terralib_examples_convertCoordinates_SOURCES = \
	convertCoordinates/convertCoordinates.cpp 

#########################################################################
# copyLayer example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_copyLayer
endif

___bin_terralib_examples_copyLayer_SOURCES = \
	copyLayer/copyLayer.cpp 

#########################################################################
# createDatabase example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createDatabase
endif

___bin_terralib_examples_createDatabase_SOURCES = \
	createDatabase/createDatabase.cpp 
	
#########################################################################
# createLayer example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createLayer
endif

___bin_terralib_examples_createLayer_SOURCES = \
	createLayer/createLayer.cpp	
	
#########################################################################
# createProximityMatrix example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createProximityMatrix
endif

___bin_terralib_examples_createProximityMatrix_SOURCES = \
	createProximityMatrix/createProximityMatrix.cpp	
		
#########################################################################
# createSTElementSet example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createSTElementSet
endif

___bin_terralib_examples_createSTElementSet_SOURCES = \
	createSTElementSet/createSTElementSet.cpp	
	
#########################################################################
# createSTElementSetFromLayer example

if TE_BUILD_SHAPELIB_BASED_EXAMPLE
if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createSTElementSetFromLayer
endif
endif

___bin_terralib_examples_createSTElementSetFromLayer_SOURCES = \
	createSTElementSetFromLayer/createSTElementSetFromLayer.cpp
	
#########################################################################
# createSTElementSetFromShapeFile example

if TE_BUILD_SHAPELIB_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createSTElementSetFromShapeFile
endif

___bin_terralib_examples_createSTElementSetFromShapeFile_SOURCES = \
	createSTElementSetFromShapeFile/createSTElementSetFromShapeFile.cpp	
	
#########################################################################
# createSTElementSetFromTheme example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createSTElementSetFromTheme
endif

___bin_terralib_examples_createSTElementSetFromTheme_SOURCES = \
	createSTElementSetFromTheme/createSTElementSetFromTheme.cpp	
	
#########################################################################
# createTable example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createTable
endif

___bin_terralib_examples_createTable_SOURCES = \
	createTable/createTable.cpp
	
#########################################################################
# createTheme example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createTheme
endif

___bin_terralib_examples_createTheme_SOURCES = \
	createTheme/createTheme.cpp	
	
#########################################################################
# createTIN example

if TE_BUILD_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_createTIN
endif

___bin_terralib_examples_createTIN_SOURCES = \
	createTIN/createTIN.cpp	

#########################################################################
# databaseQuery example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_databaseQuery
endif

___bin_terralib_examples_databaseQuery_SOURCES = \
	databaseQuery/databaseQuery.cpp		
	
#########################################################################
# databaseSQLQuery example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_databaseSQLQuery
endif

___bin_terralib_examples_databaseSQLQuery_SOURCES = \
	databaseSQLQuery/databaseSQLQuery.cpp		
	
#########################################################################
# importCSV example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_importCSV
endif

___bin_terralib_examples_importCSV_SOURCES = \
	importCSV/importCSV.cpp						
				
#########################################################################
# importDBF example

if TE_BUILD_SHAPELIB_BASED_EXAMPLE
if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_importDBF
endif
endif

___bin_terralib_examples_importDBF_SOURCES = \
	importDBF/importDBF.cpp		
	
#########################################################################
# importGeoTab example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_importGeoTab
endif

___bin_terralib_examples_importGeoTab_SOURCES = \
	importGeoTab/importGeoTab.cpp	
	
#########################################################################
# importGridData example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_importGridData
endif

___bin_terralib_examples_importGridData_SOURCES = \
	importGridData/importGridData.cpp
	
#########################################################################
# importJPEG example

if TE_BUILD_MYSQL_BASED_EXAMPLE
if TE_BUILD_JPEG_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_importJPEG
endif
endif

___bin_terralib_examples_importJPEG_SOURCES = \
	importJPEG/importJPEG.cpp	
	
#########################################################################
# importMIDMIF example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_importMIDMIF
endif

___bin_terralib_examples_importMIDMIF_SOURCES = \
	importMIDMIF/importMIDMIF.cpp		
	
#########################################################################
# importShape example

if TE_BUILD_SHAPELIB_BASED_EXAMPLE
if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_importShape
endif
endif

___bin_terralib_examples_importShape_SOURCES = \
	importShape/importShape.cpp	
	
#########################################################################
# mosaicTIFFImages example

if TE_BUILD_MYSQL_BASED_EXAMPLE
if TE_BUILD_GEOTIFF_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_mosaicTIFFImages
endif
endif

___bin_terralib_examples_mosaicTIFFImages_SOURCES = \
	mosaicTIFFImages/mosaicTIFFImages.cpp		
	
#########################################################################
# proxMatrixAndSpatialStatistics example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_proxMatrixAndSpatialStatistics
endif

___bin_terralib_examples_proxMatrixAndSpatialStatistics_SOURCES = \
	proxMatrixAndSpatialStatistics/proxMatrixAndSpatialStatistics.cpp		
					
#########################################################################
# querierFromLayer example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_querierFromLayer
endif

___bin_terralib_examples_querierFromLayer_SOURCES = \
	querierFromLayer/querierFromLayer.cpp
	
#########################################################################
# querierFromTheme example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_querierFromTheme
endif

___bin_terralib_examples_querierFromTheme_SOURCES = \
	querierFromTheme/querierFromTheme.cpp	
	
#########################################################################
# querierGroupChronon example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_querierGroupChronon
endif

___bin_terralib_examples_querierGroupChronon_SOURCES = \
	querierGroupChronon/querierGroupChronon.cpp
	
#########################################################################
# querierGroupElement example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_querierGroupElement
endif

___bin_terralib_examples_querierGroupElement_SOURCES = \
	querierGroupElement/querierGroupElement.cpp		
	
#########################################################################
# querierGroupSpatialRest example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_querierGroupSpatialRest
endif

___bin_terralib_examples_querierGroupSpatialRest_SOURCES = \
	querierGroupSpatialRest/querierGroupSpatialRest.cpp

#########################################################################
# querierWithSpatialRestBox example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_querierWithSpatialRestBox
endif

___bin_terralib_examples_querierWithSpatialRestBox_SOURCES = \
	querierWithSpatialRestBox/querierWithSpatialRestBox.cpp		
	
#########################################################################
# querierWithSpatialRestGeometry example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_querierWithSpatialRestGeometry
endif

___bin_terralib_examples_querierWithSpatialRestGeometry_SOURCES = \
	querierWithSpatialRestGeometry/querierWithSpatialRestGeometry.cpp	

#########################################################################
# rasterSlicing example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_rasterSlicing
endif

___bin_terralib_examples_rasterSlicing_SOURCES = \
	rasterSlicing/rasterSlicing.cpp

#########################################################################
# spatialQuery example

if TE_BUILD_SHAPELIB_BASED_EXAMPLE
if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_spatialQuery
endif
endif

___bin_terralib_examples_spatialQuery_SOURCES = \
	spatialQuery/spatialQuery.cpp
	
#########################################################################
# spatialQueryAndBuffer example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_spatialQueryAndBuffer
endif

___bin_terralib_examples_spatialQueryAndBuffer_SOURCES = \
	spatialQueryAndBuffer/spatialQueryAndBuffer.cpp	
	
#########################################################################
# themeGrouping example

if TE_BUILD_MYSQL_BASED_EXAMPLE
bin_PROGRAMS += ../bin/terralib_examples_themeGrouping
endif

___bin_terralib_examples_themeGrouping_SOURCES = \
	themeGrouping/themeGrouping.cpp		
			
			