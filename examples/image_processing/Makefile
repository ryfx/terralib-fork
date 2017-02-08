buildtests: 
	qmake -o Makefile_examples;
	make -f Makefile_examples;
  
distclean: 
	qmake -o Makefile_examples;
	make -f Makefile_examples distclean;	

clean : distclean	

runalltests :	
	./run_all_tests_unix.sh;	
	
check : distclean buildtests runalltests	

distcheck : check	

all : distclean buildtests
	
  

