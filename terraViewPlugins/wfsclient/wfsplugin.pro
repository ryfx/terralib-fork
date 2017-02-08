TEPATH = ../..
TEOGCPATH = ../$${TEPATH}/terraogc
TEOGCBUILDPATH = $${TEOGCPATH}/build/linux

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += $${TEOGCBUILDPATH}/common \
		$${TEOGCBUILDPATH}/xml \
		$${TEOGCBUILDPATH}/xacml \
	  $${TEOGCBUILDPATH}/ows \
	  $${TEOGCBUILDPATH}/owstheme \
	  $${TEOGCBUILDPATH}/owsclient \
	  $${TEOGCBUILDPATH}/ssw \
	  $${TEOGCBUILDPATH}/xsd \
	  $${TEOGCBUILDPATH}/gml \
	  $${TEOGCBUILDPATH}/filter \
	  $${TEOGCBUILDPATH}/se \
	  $${TEOGCBUILDPATH}/sld \
	  $${TEOGCBUILDPATH}/wfs \
	  $${TEOGCBUILDPATH}/wfsclient \
	  $${TEOGCBUILDPATH}/wfstheme 
		