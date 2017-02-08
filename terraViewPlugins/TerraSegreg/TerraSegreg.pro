#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------

TARGET = TerraSegreg

include( ../base/base.pro )

TRANSLATIONS = terrasegreg_pt.ts

INCLUDEPATH += ./include \
			   $$TERRALIBPATH/src/terralib/utils
			   
win32 {
	QMAKE_LIBDIR += $$TERRALIBPATH/$$BUILDLOC/te_utils
	LIBS += -lte_utils
}

HEADERS +=  ./include/terrasegregplugin.h \
			./include/terrasegregPluginCode.h \
			./include/TeProxMatrixGaussianStrategy.h \
			./include/TeProxMatrixBiSquaredStrategy.h \
			./include/LocalPopulationIntensity.h \
			./include/LocalPopulationIntensityWin.h \
			./include/SpatialIndex.h \
			./include/SpatialIndexWin.h \
			./include/GeneralizedDissimilarityIndex.h \
			./include/GeneralizedDissimilarityIndexWin.h \
			./include/NeighborhoodSortingIndex.h \
			./include/NeighborhoodSortingIndexWin.h \
			./include/ExposureIndex.h \
			./include/ExposureIndexWin.h \
			./include/ExposureIndexMN.h \
			./include/ExposureIndexMNWin.h \
			./include/IsolationIndexM.h \
			./include/IsolationIndexMWin.h \
			./include/Utils.h

SOURCES +=  ./src/terrasegregplugin.cpp \
			./src/terrasegregPluginCode.cpp \
			./src/TeProxMatrixGaussianStrategy.cpp \
			./src/TeProxMatrixBiSquaredStrategy.cpp \
			./src/LocalPopulationIntensity.cpp \
			./src/LocalPopulationIntensityWin.cpp \
			./src/SpatialIndex.cpp \
			./src/SpatialIndexWin.cpp \
			./src/GeneralizedDissimilarityIndex.cpp \
			./src/GeneralizedDissimilarityIndexWin.cpp \
			./src/NeighborhoodSortingIndex.cpp \
			./src/NeighborhoodSortingIndexWin.cpp \
			./src/ExposureIndex.cpp \
			./src/ExposureIndexWin.cpp \
			./src/ExposureIndexMN.cpp \
			./src/ExposureIndexMNWin.cpp \
			./src/IsolationIndexM.cpp \
			./src/IsolationIndexMWin.cpp \
			./src/Utils.cpp
			
FORMS += ./ui/UIAbout.ui \
		 ./ui/UILocalPopulationIntensity.ui \
		 ./ui/UISpatialIndex.ui

IMAGES	= ./images/TerraSegreg.png
