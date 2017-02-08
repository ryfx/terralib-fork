#!/bin/bash

echo Starting test procedure...
echo The current directory is:
pwd

echo Cleanning garbage files...
find -name "*.tif" -exec rm -f {} \;
find -name "*.ilk" -exec rm -f {} \;
find -name "*.pdb" -exec rm -f {} \;
find -name "*.exe" -exec rm -f {} \;
echo Cleanning done.

TEST_FAILED=0
AT_LEAST_ONE_TEST_FAILED=0

for i in $( ls TePDI* ); do
  if [ -x $i ]; then
    echo Now running $i test ...
    
    TEST_FAILED=0
    /bin/nice -n +5 $i &> /tmp/terralibupdate.log
    
    if [ "$?" -ne "0" ]; then 
      TEST_FAILED=1
      AT_LEAST_ONE_TEST_FAILED=1
      echo Test $i failed.
    fi
    
    if [ -n "$1" ]; then
      echo ----------------------------------------------------------------
      cat /tmp/terralibupdate.log
      echo ----------------------------------------------------------------    
    else
      if [ $TEST_FAILED -eq "1" ]; then
        echo ----------------------------------------------------------------
        cat /tmp/terralibupdate.log
        echo ----------------------------------------------------------------    
      fi      
    fi
  fi
done
if [ $AT_LEAST_ONE_TEST_FAILED -ne "0" ]; then
  echo ----------------------------------------------------------------
  echo Test procedure finished with errors.
else
  echo Test procedure finished with no errors.
fi
