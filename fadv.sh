#!/bin/bash
MEMORY_TEST_HOME=/home/user/memory-test
cd $MEMORY_TEST_HOME
while [ 1 ] 
do
	for i in {1..13}
	do
		file_name=10G_$i.file
		echo 'fadving ' $file_name
		file_name=10G_$i.file
		./fadv $file_name
	done
	#sleep 5
	break
done
