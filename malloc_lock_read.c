#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/time.h>

#define BLOCK_SIZE 1048576

unsigned int parse_memory_size(char *m);


struct mem_block
{
	char buffer[BLOCK_SIZE];
};

int main (int argc, char *argv[])
{
	printf("PID: %d\n", (int) getpid());
	if (argc < 2)
	{
		printf ("please specify the amout of memory you want to allocate\n");
		return 0;
	}
	uid_t uid = getuid();
	if (setuid(0))
	{
		printf("cannot change uid to 0. exiting.\n");
		return -1;
	}
	mlockall(MCL_FUTURE);
	unsigned int memory_to_alloc_in_gb = atoi(argv[1]);

	// count the number of blocks in MB unit
	int numOfBlock = memory_to_alloc_in_gb * 1024;

	struct mem_block* blocks[numOfBlock];

	//printf("size of mem_block: %lu\n", sizeof(blocks));
	for (int i = 0; i < numOfBlock; i++)
	{
		struct mem_block* block = malloc(sizeof(struct mem_block));
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			block -> buffer[j] = 'a';
		}
		blocks[i] = block;
	}


	printf("memory allocated and locked\n");


	struct timeval start, end;
	unsigned long timer;
	gettimeofday(&start, NULL);
	//mlockall(MCL_CURRENT);
	while(1)
	{
		//randomly choose a row to access
		int random_row = rand() % numOfBlock;
		//start = (double)clock();
		gettimeofday(&start, NULL);
		struct mem_block* block = blocks[random_row];
		char tmp;
		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			tmp = block -> buffer[i];
		}
		//end = (double)clock();
		gettimeofday(&end, NULL);
		//printf("reading a random row %d\ttakes: %f seconds.\n", random_row, (end - start) / CLOCKS_PER_SEC);
		timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
		printf("reading a random row %d\ttakes: %ld us.\n", random_row, timer);
		sleep(1);
	}

	while(1)
	{
		char exit;
		printf("exit? y/n\n");
		scanf("%c", &exit);
		if (exit == 'y' || exit == 'Y')
		{
			return 0;
		}
	}
}
	
