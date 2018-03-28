#!/bin/sh
for i in {0..167}
do
  printf -v i "%.3d" $i
  echo "input.bin.$i"
  diff "input.bin.$i" ./replacement/"input.bin.$i"
  #diff input.bin.
done
diff "sort.bin" "sorted.bin"