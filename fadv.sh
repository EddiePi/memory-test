#!/bin/bash
MEMORY_TEST_HOME=/home/eddie/memory-test
cd $MEMORY_TEST_HOME
while [ 1 ] 
do
	for i in {0..15}
	do
		file_name=1G_$i.file
		echo 'fadving ' $file_name
		./fadv $file_name
	done
	#sleep 5
	break
done
