TARGET = spatialQuery

include( ../base/base.pro )

LIBS += -lte_shapelib

SOURCES	+= \
	spatialQuery.cpp