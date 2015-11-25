#!/bin/sh

FILES=`find include src -name '*.cpp' -print , -name '*.h' -print`

for i in $FILES
do
    echo $i
    if ! grep -q Copyright $i
    then
      cat copyright.txt > $i.new
      echo >> $i.new
      cat $i >> $i.new
      mv $i.new $i
    fi
done
