#PATH definitions
TOGCPATH=../../../terraogc
TOGCBUILD=$$TOGCPATH/build/linux
#----------------
TEMPLATE=subdirs
SUBDIRS = $$TOGCBUILD/common \
		$$TOGCBUILD/xml \
		$$TOGCBUILD/xsd \
		$$TOGCBUILD/gml \
		$$TOGCBUILD/ows \
		$$TOGCBUILD/filter \
		$$TOGCBUILD/wms \
		$$TOGCBUILD/se \
		$$TOGCBUILD/sld \
		$$TOGCBUILD/xacml \
		$$TOGCBUILD/owsclient \
		$$TOGCBUILD/wmsclient \
		$$TOGCBUILD/owstheme \
		$$TOGCBUILD/wmstheme