#!/bin/bash

matrix_size="4 8 16"
child_number="1 2 4"

total_child=0
tmp=0

for m in $matrix_size
do
	for t in $child_number
	do	
		echo "Execute matrixmul $m $t"
		tmp=$(./matrixmul $m $t | grep -o Child | wc -l)
		# Fill here
		total_child=$(($total_child+$tmp))
	done
done

echo "Total $total_child children are used to preform matrix multiplication"



