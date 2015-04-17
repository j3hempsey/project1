#!/bin/bash

matrix_size="4 8 16"
child_number="1 2 4"

total_child=0
tmp=0

for m in #Fill here
do
	for t in $child_number
	do	
		echo "Execute matrixmul $m $t"
		tmp=$(./Fill here | grep -o Child | wc -l)
		# Fill here
		total_child=$((...+...))
	done
done

# Fill here: print total how many child processes
# were used to calculate matrix multiplication
echo "..."



