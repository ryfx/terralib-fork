# ------------------------------- #
# TerraView GeoDMA cmake project. #
# (c) Thales Sehn Korting, 2011   #
# ------------------------------- #

cmake_minimum_required(VERSION 2.8)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../common)

#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/plugins)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/../../Release/plugins)
#SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/plugins)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/../../Debug/plugins)

SET(TVPLGS_DIR "${TVPLGS_DIR}/geodma") # change this if terralib is somewhere else

SET(PROJ_NAME "geodma")

PROJECT(${PROJ_NAME})

SET(TE_DIR "${TVPLGS_DIR}/../../src/terralib")
SET(TV_DIR "${TVPLGS_DIR}/../../terraView")

find_package(TERRALIB REQUIRED)
#find_package(TERRALIB REQUIRED COMPONENTS qwt3_drivers)
find_package(QWT REQUIRED)

#if((NOT DEFINED TE_DIRS) OR (NOT DEFINED TE_LIBS))
#  find_package(terralib REQUIRED)
#endif()

include_directories (
  ${TE_DIR}
  ${TE_DIR}/drivers/qt
  ${TE_DIR}/drivers/spl
  ${TE_DIR}/functions
  ${TE_DIR}/image_processing
  ${TE_DIR}/kernel
  ${TE_DIR}/../libspl
  ${TV_DIR}
  ${TVPLGS_DIR}
  ${TVPLGS_DIR}/images
  ${TVPLGS_DIR}/include
  ${QT_INCLUDE_DIR}
  ${QWT_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/../../terraViewCore
)

# Qt files
# Requires Qt3 multi-threaded
SET(QT_MT_REQUIRED, TRUE)
FIND_PACKAGE(Qt3 REQUIRED)

SET(UI_DIR "${TVPLGS_DIR}/ui")
SET(INC_DIR "${TVPLGS_DIR}/include")
SET(SRC_DIR "${TVPLGS_DIR}/src")

# Configuring files to uic
FILE(GLOB UI_FILES ${UI_DIR}/*.ui)
#FILE(GLOB H_UI_FILES ${UI_DIR}/*.h)
QT_WRAP_UI(${PROJ_NAME} PLG_UI_HDRS PLG_UI_SRCS ${UI_FILES})

# Configuring files to moc
SET(TOMOC_LIST
  ${TVPLGS_DIR}/include/geodma_plugin.h
  #${TVPLGS_DIR}/ui/geodma_window.h
)
QT_WRAP_CPP(${PROJ_NAME} MOC_SRCS ${TOMOC_LIST})

SET(_DIRS .)

getFfiles("${INC_DIR}" "${_DIRS}" h_files true "")
getFfiles("${SRC_DIR}" "${_DIRS}" srcs_files false "")

# FILE(GLOB UI_SRCS ${UI_DIR}/*.h)

SET(SRCS ${h_files} ${srcs_files} ${UI_FILES} ${MOC_SRCS} ${PLG_UI_HDRS} ${PLG_UI_SRCS}) ## ${H_UI_FILES} ${UI_SRCS}

# Ui filter
source_group("UI Files" FILES ${UI_FILES}) ## ${H_UI_FILES}
source_group("Generated UI Files" FILES ${PLG_UI_HDRS} ${PLG_UI_SRCS})
source_group("Generated MOC Files" FILES ${MOC_SRCS})
#source_group("Source Files" FILES ${UI_SRCS})

LIST(APPEND TSLIST "geodma_pt" "geodma_es")

SET(PRO_FILENAME ${TVPLGS_DIR}/${PROJ_NAME}.pro)
foreach(TS ${TSLIST})
  SET(TSFILES "${TSFILES} ${TVPLGS_DIR}/${TS}.ts")
endforeach()

generatePRO(${PRO_FILENAME} "${h_files}" "${srcs_files}" "${UI_FILES}" "${TSFILES}")

foreach(TS ${TSLIST})
  SET(QMFILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${TS}.qm")
  SET(TSFILE "${TVPLGS_DIR}/${TS}.ts")
  generateTS(${TSFILE} ${PRO_FILENAME})
  generateQM(${TSFILE} ${QMFILE})
endforeach()

# list ( APPEND TV_LIBS ${QT_LIBRARIES} ${QWT_LIBRARY} )

# -----------------------------------

add_definitions(${QT_DEFINITIONS} -DQT_THREAD_SUPPORT)

add_library(${PROJ_NAME} SHARED ${SRCS})

target_link_libraries(${PROJ_NAME} terralib terraViewCore)
