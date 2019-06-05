#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
#include <string>
#include <sys/time.h>
#include <stdio.h>

using namespace std;

int main(int argv, char **argc)
{	
	struct timeval start;
	struct timeval end;
	int pid = getpid();
	cout << "mmap test start, pid: " << pid <<endl;
	int SIZE = 1024 * 1024;
	bool POPULATE = 0;
	if (argv >= 2)
	{
		SIZE *= atoi(argc[1]);
	}
	if (argv >= 3)
	{
		POPULATE = atoi(argc[2]);
	}
	cout << "press enter to start mmap" << endl;
	char dummy;
	dummy = getchar();
	int mask = MAP_PRIVATE | MAP_ANONYMOUS;
	if (POPULATE != 0)
	{
		mask = mask | MAP_POPULATE;
		cout << "populate mmap" << endl;
	}
	else 
	{
		cout << "not populate mmap" << endl;
	}
	gettimeofday(&start, NULL);
	char* space = (char*) mmap (NULL, SIZE*sizeof(char), PROT_READ | PROT_WRITE, mask, -1, 0);
	if (space == MAP_FAILED)
	{
		cout << "mmap() failed: " << errno <<endl;
	}
	for (int i = 0; i < SIZE; i++)
	{
		if (i % 1000000 == 0)
		{
			usleep(50);
		}
		space[i] = 'a';
	}
	gettimeofday(&end, NULL);
	cout << "press enter to get mmap status" << endl;
	dummy = getchar();
	cout << "execution time " << (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec) << " us" << endl;
	return 0;
}
