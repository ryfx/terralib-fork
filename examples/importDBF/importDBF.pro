TARGET = importDBF

include( ../base/base.pro )

LIBS += -lte_shapelib

SOURCES	+= \
	importDBF.cpp
