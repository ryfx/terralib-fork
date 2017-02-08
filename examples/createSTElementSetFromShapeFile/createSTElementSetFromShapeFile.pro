TARGET = createSTElementSetFromShapeFile

include( ../base/base.pro )

LIBS += -lte_shapelib

SOURCES	+= \
	createSTElementSetFromShapeFile.cpp
