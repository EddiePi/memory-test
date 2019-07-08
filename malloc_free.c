#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include<stdio.h>

unsigned int parse_memory_size(char *m);

struct mem_block
{
	char buffer[1048576];
};

int main (int argc, char *argv[])
{
	printf("PID: %d\n", (int) getpid());
	if (argc < 2)
	{
		printf ("please specify the amout of memory (MB) you want to allocate");
		return 0;
	}

	unsigned int memory_to_alloc_in_mb = atoi(argv[1]);
	
	// count the number of blocks in MB unit
	int numOfBlock = memory_to_alloc_in_mb;

	struct mem_block* blocks[numOfBlock];
	//printf("size of mem_block: %lu\n", sizeof(blocks));
	struct timeval start, alloc_end, end;
	unsigned long alloc_timer, timer;
	while(1)
	{	
		char dummy;
		printf("press enter to start allocation.\n");
		dummy = getchar();
		gettimeofday(&start, NULL);
		// allocate the memory
		for (int i = 0; i < numOfBlock; i++)
		{
			struct mem_block* block = malloc(sizeof(struct mem_block));
			for (int j = 0; j < 1048576; j++)
			{
				block -> buffer[j] = 'a';
			}
			blocks[i] = block;
		}
		gettimeofday(&alloc_end, NULL);
		printf("press enter to free the memory\n");
		dummy = getchar();

		// free the memory
		for (int i = 0; i < numOfBlock; i++)
		{
			free(blocks[i]);
		}
		gettimeofday(&end, NULL);
		alloc_timer = 1000000 * (alloc_end.tv_sec - start.tv_sec) + alloc_end.tv_usec - start.tv_usec;
		timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
		printf("one alloc-free round end. alloc time %ld us. total time %ld us\n", alloc_timer, timer);
		while(1)
		{
			char exit;
			printf("exit? 'y', alloc again? 'a'\n");
			scanf("%c", &exit);
			if (exit == 'y' || exit == 'Y')
			{
				return 0;
			} else if (exit == 'a' || exit == 'A')
			{
				break;
			}
		}
	}
}
	
