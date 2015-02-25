#!/bin/sh

# iterate tough each of the tests in the test directory
# and run the check against it
for file in $(ls tests); do

  # display the file name
  echo $file

  # run the checker against the file
  echo 'checker-serial'
  time ./checker-serial < ./tests/$file > /dev/null
  echo 'checker-parallel'
  time ./checker-parallel < ./tests/$file > /dev/null

done
