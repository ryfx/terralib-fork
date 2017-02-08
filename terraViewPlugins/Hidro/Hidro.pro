#--------------------------------------------------------------------------------------------------------
#If you want to create a visual studio project, use the commmand "qmake -t vclib -spec win32-msvc.net".
#For debug version add the expression "TE_PROJECT_TYPE=DEBUG" to previous command.
#--------------------------------------------------------------------------------------------------------

TARGET = Hidro
include( ../base/base.pro )

TRANSLATIONS = \
	hidro_pt.ts \
	hidro_sp.ts

INCLUDEPATH += \
	./src/boost \
	./src/boost/algorithms \
	./src/classification \
	./src/config \
	./src/edition \
	./src/flow \
	./src/functions \
	./src/kernel \
\
	$$TERRALIBPATH/src/terralib/image_processing \
	$$TERRALIBPATH/src/terralib/utils
	
QMAKE_LIBDIR += \
	$$TERRALIBPATH/$$BUILDLOC/terralibpdi \
	$$TERRALIBPATH/$$BUILDLOC/te_utils
	
LIBS += \
	-lterralibpdi \
	-lte_utils	

HEADERS += \
	./src/boost/HidroBoostAlgorithmFactory.h \
	./src/boost/HidroBoostAlgorithms.h \
	./src/boost/HidroBoostParams.h \
	./src/boost/HidroBoostUtils.h \
  	./src/boost/algorithms/HidroBoostBidirectionGraph.h \
	./src/boost/algorithms/HidroBoostBidirectionGraphFactory.h \
	./src/boost/algorithms/HidroBoostBreadthFirstSearch.h \
	./src/boost/algorithms/HidroBoostBreadthFirstSearchFactory.h \
	./src/boost/algorithms/HidroBoostConnectedComponents.h \
	./src/boost/algorithms/HidroBoostConnectedComponentsFactory.h \
	./src/boost/algorithms/HidroBoostDijkstra.h \
	./src/boost/algorithms/HidroBoostDijkstraFactory.h \
	./src/boost/algorithms/HidroBoostIncidenceGraph.h \
	./src/boost/algorithms/HidroBoostIncidenceGraphFactory.h \
	./src/boost/algorithms/HidroBoostStronglyConnectedComponents.h \
	./src/boost/algorithms/HidroBoostStronglyConnectedComponentsFactory.h \
\
	./src/classification/HidroCrossTable.h \
	./src/classification/HidroOverlay.h \
	./src/classification/HidroOverlayClass.h \
	./src/classification/HidroOverlayClassGroup.h \
	./src/classification/HidroOverlayRule.h \
	./src/classification/HidroOverlayRuleGroup.h \
\
	./src/config/hidroplugin.h \
	./src/config/hidroPluginCode.h \
	./src/config/HidroPluginStarter.h \
\
	./src/edition/HidroEdgeEdition.h \
	./src/edition/HidroGraphEdition.h \
	./src/edition/HidroOptimizedEdition.h \
	./src/edition/HidroVertexEdition.h \
\
	./src/flow/HidroFix.h \
	./src/flow/HidroFixPFS.h \
	./src/flow/HidroFlowD8.h \	
	./src/flow/HidroFlowDEM8Connected.h \
	./src/flow/HidroFlowUtils.h \
	./src/flow/HidroMatrix.hpp \
	./src/flow/HidroPFS.h \
	./src/flow/HidroPitCorrectionD8MediaQueue.h \	
	./src/flow/HidroSmartQueue.h \
\
	./src/functions/HidroConvexHull.h \
	./src/functions/HidroEdgeFunctions.h \
	./src/functions/HidroGroupLUTController.h \
	./src/functions/HidroLUTController.h \
	./src/functions/HidroMetadata.h \
	./src/functions/HidroPersister.h \
	./src/functions/HidroUpscale.h \
	./src/functions/HidroUtils.h \
	./src/functions/HidroVertexFunctions.h \
\
	./src/kernel/HidroAttr.h \
	./src/kernel/HidroAttrDouble.h \
	./src/kernel/HidroAttrInt.h \
	./src/kernel/HidroAttrString.h \
	./src/kernel/HidroEdge.h \
	./src/kernel/HidroGraph.h \
	./src/kernel/HidroVertex.h
	

SOURCES += \
	./src/boost/HidroBoostAlgorithmFactory.cpp \
	./src/boost/HidroBoostAlgorithms.cpp \
	./src/boost/HidroBoostParams.cpp \
	./src/boost/HidroBoostUtils.cpp \
  	./src/boost/algorithms/HidroBoostBidirectionGraph.cpp \
	./src/boost/algorithms/HidroBoostBidirectionGraphFactory.cpp \
	./src/boost/algorithms/HidroBoostBreadthFirstSearch.cpp \
	./src/boost/algorithms/HidroBoostBreadthFirstSearchFactory.cpp \
	./src/boost/algorithms/HidroBoostConnectedComponents.cpp \
	./src/boost/algorithms/HidroBoostConnectedComponentsFactory.cpp \
	./src/boost/algorithms/HidroBoostDijkstra.cpp \
	./src/boost/algorithms/HidroBoostDijkstraFactory.cpp \
	./src/boost/algorithms/HidroBoostIncidenceGraph.cpp \
	./src/boost/algorithms/HidroBoostIncidenceGraphFactory.cpp \
	./src/boost/algorithms/HidroBoostStronglyConnectedComponents.cpp \
	./src/boost/algorithms/HidroBoostStronglyConnectedComponentsFactory.cpp \
\
	./src/classification/HidroCrossTable.cpp \
	./src/classification/HidroOverlay.cpp \
	./src/classification/HidroOverlayClass.cpp \
	./src/classification/HidroOverlayClassGroup.cpp \
	./src/classification/HidroOverlayRule.cpp \
	./src/classification/HidroOverlayRuleGroup.cpp \
\
	./src/config/hidroplugin.cpp \
	./src/config/hidroPluginCode.cpp \
	./src/config/HidroPluginStarter.cpp \
\
	./src/edition/HidroEdgeEdition.cpp \
	./src/edition/HidroGraphEdition.cpp \
	./src/edition/HidroOptimizedEdition.cpp \
	./src/edition/HidroVertexEdition.cpp \
\
	./src/flow/HidroFix.cpp \
	./src/flow/HidroFixPFS.cpp \
	./src/flow/HidroFlowD8.cpp \	
	./src/flow/HidroFlowDEM8Connected.cpp \
	./src/flow/HidroFlowUtils.cpp \
	./src/flow/HidroPFS.cpp \
	./src/flow/HidroPitCorrectionD8MediaQueue.cpp \	
	./src/flow/HidroSmartQueue.cpp \
\
	./src/functions/HidroConvexHull.cpp \
	./src/functions/HidroEdgeFunctions.cpp \
	./src/functions/HidroGroupLUTController.cpp \
	./src/functions/HidroLUTController.cpp \
	./src/functions/HidroMetadata.cpp \
	./src/functions/HidroPersister.cpp \
	./src/functions/HidroUpscale.cpp \
	./src/functions/HidroUtils.cpp \
	./src/functions/HidroVertexFunctions.cpp \
\
	./src/kernel/HidroAttr.cpp \
	./src/kernel/HidroAttrDouble.cpp \
	./src/kernel/HidroAttrInt.cpp \
	./src/kernel/HidroAttrString.cpp \
	./src/kernel/HidroEdge.cpp \
	./src/kernel/HidroGraph.cpp \
	./src/kernel/HidroVertex.cpp

FORMS += \
	./ui/HidroAttributeWindow.ui \
	./ui/HidroBoostUIBidirectionalGraph.ui \
	./ui/HidroBoostUIConnectedComponents.ui \
	./ui/HidroBoostUIDijkstra.ui \
	./ui/HidroBoostUISearch.ui \
	./ui/HidroClassWindow.ui \
	./ui/HidroEditionWindow.ui \
	./ui/HidroMainWindow.ui \
	./ui/HidroMaskWindow.ui \
	./ui/HidroOptimizedEditionWindow.ui

IMAGES = \
	./ui/images/arrow-downleft_green.bmp \
	./ui/images/arrow-downleft_red.bmp \
	./ui/images/arrow-downleft_yellow.bmp \
	./ui/images/arrow-downright_green.bmp \
	./ui/images/arrow-downright_red.bmp \
	./ui/images/arrow-downright_yellow.bmp \
	./ui/images/arrow-down_green.bmp \
	./ui/images/arrow-down_red.bmp \
	./ui/images/arrow-down_yellow.bmp \
	./ui/images/arrow-left_green.bmp \
	./ui/images/arrow-left_red.bmp \
	./ui/images/arrow-left_yellow.bmp \
	./ui/images/arrow-right_green.bmp \
	./ui/images/arrow-right_red.bmp \
	./ui/images/arrow-right_yellow.bmp \
	./ui/images/arrow-upleft_green.bmp \
	./ui/images/arrow-upleft_red.bmp \
	./ui/images/arrow-upleft_yellow.bmp \
	./ui/images/arrow-upright_green.bmp \
	./ui/images/arrow-upright_red.bmp \
	./ui/images/arrow-upright_yellow.bmp \
	./ui/images/arrow-up_green.bmp \
	./ui/images/arrow-up_red.bmp \
	./ui/images/arrow-up_yellow.bmp \
	./ui/images/checkFalse.bmp \
	./ui/images/checkTrue.bmp \
	./ui/images/clear.png \
	./ui/images/delete.png \
	./ui/images/hidroIcon.bmp \
	./ui/images/hidroIconAttribute.bmp \
	./ui/images/hidroIconClass.bmp \
	./ui/images/hidroIconEdition.bmp \
	./ui/images/hidroIconMain.bmp \
	./ui/images/ok.png \
	./ui/images/select.png \
	./ui/images/terraLibHidro.png \
	./ui/images/undo.png \

win32 {
	release {
        QMAKE_POST_LINK += CopyReleasePlugin.bat
    }
}