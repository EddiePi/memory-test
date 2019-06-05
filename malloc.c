#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
		printf ("please specify the amout of memory you want to allocate");
		return 0;
	}

	unsigned int memory_to_alloc_in_gb = atoi(argv[1]);

	// count the number of blocks in MB unit
	int numOfBlock = memory_to_alloc_in_gb * 1024;

	struct mem_block* blocks[numOfBlock];
	//printf("size of mem_block: %lu\n", sizeof(blocks));
	for (int i = 0; i < numOfBlock; i++)
	{
		struct mem_block* block = malloc(sizeof(struct mem_block));
		for (int j = 0; j < 1048576; j++)
		{
			block -> buffer[j] = 'a';
		}
		blocks[i] = block;
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
	
