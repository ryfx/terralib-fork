#!/bin/bash

echo Starting test procedure...

cd bin
echo The current directory is:
pwd

echo Cleanning garbage files...
find -name "*.tif" -exec rm -f {} \;
find -name "*.ilk" -exec rm -f {} \;
find -name "*.pdb" -exec rm -f {} \;
find -name "*.exe" -exec rm -f {} \;
echo Cleanning done.

AT_LEAST_ONE_TEST_FAILED=0
LOGFILE=/tmp/pditestsunit.log

for i in $( ls TePDI* ); do
  if [ -x $i ]; then
    echo Now running $i test ...
    
    TEST_FAILED=0
    rm -f $LOGFILE
    
    ./$i &> $LOGFILE
    
    if [ "$?" -ne "0" ]; then 
      TEST_FAILED=1
      AT_LEAST_ONE_TEST_FAILED=1
      echo Test $i failed.
      
      echo ----------------------------------------------------------------
      cat $LOGFILE
      echo ----------------------------------------------------------------       
    else
      echo Test $i OK.
    fi
  fi
done

if [ $AT_LEAST_ONE_TEST_FAILED -ne "0" ]; then
  echo ----------------------------------------------------------------
  echo Test procedure finished with errors.
  cd ..
  exit 1
else
  echo Test procedure finished with no errors.
  cd ..
  exit 0
fi
