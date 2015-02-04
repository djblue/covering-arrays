#!/bin/sh

# iterate tough each of the tests in the test directory
# and run the check against it
for file in $(ls tests); do

  # display the file name
  echo $file
  # run the checker against the file
  ./checker < ./tests/$file

done
