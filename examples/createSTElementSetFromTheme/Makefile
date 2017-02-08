all: createSTElementSetFromTheme

terralib:
	cd ../../terralibx/terralib; make

createSTElementSetFromTheme: terralib
	qmake -o makeCreateSTElementSetFromTheme createSTElementSetFromTheme.pro; make -f makeCreateSTElementSetFromTheme

clean:
	if \
		test -f makeCreateSTElementSetFromTheme; \
	then \
		make -f makeCreateSTElementSetFromTheme distclean; rm -f makeCreateSTElementSetFromTheme; \
	fi