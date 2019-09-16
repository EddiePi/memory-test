#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>


struct mmap_node 
{
	void* addr;
	size_t length;
	struct mmap_node* next;
	struct mmap_node* prev;
};
void fill_chunk(void* addr, unsigned long length);

struct mmap_node head;

int mmap_sys(size_t length, struct mmap_node *head)
{
	struct timeval start;
	struct timeval end;

	gettimeofday(&start, NULL);
	//void* space = mmap (NULL, length*sizeof(char), 
	//			PROT_READ | PROT_WRITE, 
	//			MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, 
	//			-1, 0);
    void* space = malloc(length);
	gettimeofday(&end, NULL);
	if (space == NULL)
	{
		printf("mmap_sys failed\n");
		return -1;
	}
	printf("mmap populate time: %ld us, length: %ld\n ", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec), length);
	
/*	struct mmap_node tmp =
	{
		.addr = space,
		.length = length,
		.prev = head,
		.next = head->next,
	};*/
	struct mmap_node *tmp = (struct mmap_node*)malloc(sizeof(struct mmap_node));
	tmp->addr = space;
	tmp->length = length;
	tmp->next = head->next;
	tmp->prev = head;
	head->next = tmp;
	tmp->next->prev = tmp;
	printf("node addr: %ld, mmap addr: %ld\n", (unsigned long)tmp, (unsigned long) space);
	return 0;
}

struct mmap_node* mmap_buf(size_t length, struct mmap_node *head, int populate)
{
	void* space;
	if (head->next == head)
	{

		// direct alloc
		int mask = 0;
		printf("poll is empty, get memory from mmap\n");
		if (populate)
		{
			mask |= MAP_POPULATE;
			printf("populate\n");
		}
		else
		{
			printf("not populate\n");
		}
		//void* space = mmap (NULL, length*sizeof(char), 
        //                       PROT_READ | PROT_WRITE, 
        //                        MAP_PRIVATE | MAP_ANONYMOUS | mask, 
        //                       -1, 0);
        
        void* space = malloc(length);
		if (space == NULL)
		{
			printf("mmap failed\n");
		}
		struct mmap_node* res = (struct mmap_node*)malloc(sizeof(struct mmap_node));
		res->addr = space;
		res->length = length;
		return res;
	};
	printf("get memory from buffer\n");
	struct mmap_node* to_go = head->next;
	to_go->prev->next = to_go->next;
	to_go->next->prev = to_go->prev;
	if (to_go->length != length)
	{
		//void *new_addr = mremap(to_go->addr, 
		//		to_go->length, 
		//		length, MREMAP_MAYMOVE);
        void* new_addr = realloc(to_go->addr, length);
		if (new_addr == NULL)
		{
			printf("remap failed\n");
			return new_addr;
		}
		// populate the remap memory if necessary
		if (populate)
		{
			fill_chunk(new_addr, length);
		}
		printf("mremap called, old length: %ld, new length: %ld\n", to_go->length, length);
		to_go->addr = new_addr;
		to_go->length = length;
	}
	printf("got buffered memory, addr %ld\n", (unsigned long)to_go->addr);
	return to_go;
}

void fill_poll(size_t length, struct mmap_node *head, int num)
{
	for(int i = 0; i < num; i++)
	{
		mmap_sys(length, head);
	}
	printf("%d mmap filled\n", num);
}

void fill_chunk(void* addr, unsigned long length)
{
	char *caddr = (char *)addr;
	for (unsigned long i = 0; i < length; i++)
	{
		caddr[i] = 0;
	}
}


void alloc_small()
{
  FILE *ofile;
  ofile = fopen("/home/eddie/memory-test/data/data.txt", "w");
  int base = 1024;
  size_t size = 1;
  int count = 10000;
  void **buf;
  char dummy;
  long wait = 0;
  struct timeval start;
  struct timeval end;
  struct timeval r_start;
  struct timeval r_end;
  struct timespec start_ns;
  struct timespec end_ns;
  printf("specify chunk size in KB\n");
  scanf("%ld", &size);
  size *= 1024;
  printf("specify # of chunks, default 10000\n");
  scanf("%d", &count);
  buf = (void **) malloc(sizeof(void *) * count);
  printf("press enter to alloc memory\n");
	while((dummy = getchar()) != EOF && dummy != '\n');
  getchar();

  gettimeofday(&start, NULL);
  for(int i = 0; i < count; i++)
  {
    //gettimeofday(&r_start, NULL);
    clock_gettime(CLOCK_REALTIME, &start_ns);
    buf[i] = malloc(size);
    char *c = (char *) buf[i];
    for (int j = 0; j < size; j++)
    {
      c[j] = 'a';
    }
    clock_gettime(CLOCK_REALTIME, &end_ns);
    //gettimeofday(&r_end, NULL);
    //fprintf(ofile, "%ld\n", (r_end.tv_sec - r_start.tv_sec) * 1000000 + (r_end.tv_usec - r_start.tv_usec));
    fprintf(ofile, "%ld\n", (end_ns.tv_sec - start_ns.tv_sec) * 1000000000 + (end_ns.tv_nsec - start_ns.tv_nsec));
    if (i % 1000 == 0)
    {
      wait += 100;
      usleep(100);
    }

  }
  /*
  gettimeofday(&end, NULL);
  printf("alloc time: %ld us\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  printf("press enter to fill memory\n");
	//while((dummy = getchar()) != EOF && dummy != '\n');
  getchar();
  gettimeofday(&start, NULL);
  for(int i = 0; i < count; i++)
  {
    char *c = (char *)buf[i];
    for (int j = 0; j < size; j++)
    {
      c[j] = 'a';
    }
  }
  */
  gettimeofday(&end, NULL);
  printf("fill time: %ld us\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec) - wait);
  printf("press enter to release memory\n");
	//while((dummy = getchar()) != EOF && dummy != '\n');
  getchar();
  for (int i = 0; i < count; i++)
  {
    free(buf[i]);
  }
  fclose(ofile);
}

void alloc_large()
{
  FILE *ofile;
  ofile = fopen("/home/eddie/memory-test/data/data.txt", "w");
  int base = 1024;
  size_t size = 1;
  int count = 10000;
  void **buf;
  char dummy;
  long wait = 0;
  struct timeval start;
  struct timeval end;
  struct timeval r_start;
  struct timeval r_end;
  printf("specify chunk size in KB (minimum 128 K)\n");
  scanf("%ld", &size);
  if (size < 128)
  {
    size = 128;
  }
  size *= 1024;
  printf("specify # of chunks, default 10000\n");
  scanf("%d", &count);
  buf = (void **) malloc(sizeof(void *) * count);
  printf("press enter to alloc memory\n");
	while((dummy = getchar()) != EOF && dummy != '\n');
  getchar();

  long total = 0;
  long each = 0;
  gettimeofday(&start, NULL);
  for(int i = 0; i < count; i++)
  {
    gettimeofday(&r_start, NULL);
    buf[i] = malloc(size);
    char *c = (char *) buf[i];
    for (int j = 0; j < size; j++)
    {
      c[j] = 'a';
    }
    gettimeofday(&r_end, NULL);
    each = (r_end.tv_sec - r_start.tv_sec) * 1000000 + (r_end.tv_usec - r_start.tv_usec);
    fprintf(ofile, "%ld\n", each);
    total += each;
    usleep(2000);
  }
  fflush(ofile);
  /*
  gettimeofday(&end, NULL);
  printf("alloc time: %ld us\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  printf("press enter to fill memory\n");
	//while((dummy = getchar()) != EOF && dummy != '\n');
  getchar();
  gettimeofday(&start, NULL);
  for(int i = 0; i < count; i++)
  {
    char *c = (char *)buf[i];
    for (int j = 0; j < size; j++)
    {
      c[j] = 'a';
    }
  }
  */
  gettimeofday(&end, NULL);
  printf("fill time: %ld us\n", total);
  printf("press enter to release memory\n");
	//while((dummy = getchar()) != EOF && dummy != '\n');
  getchar();
  for (int i = 0; i < count; i++)
  {
    free(buf[i]);
  }
  fclose(ofile);
}


void show_all_mmap(struct mmap_node *head)
{
	struct mmap_node *cur = head->next;
	int i = 0;
	while(cur != head)
	{
		printf("node: %d, addr %ld\n", i++, (unsigned long)cur->addr);
		cur = cur->next;
	}
}

void show_menu()
{
	printf("1: get memory\n");
	printf("2: refill poll\n");
	printf("3: exit\n");
	printf("4: show all mmap\n");
  printf("5: allocate small chunk of memory\n");
  printf("6: allocate large chunk of memory\n");
}


int main(int argv, char **argc)
{	
	int prepared_num = 1;
	struct timeval start;
	struct timeval end;
	int pid = getpid();
	printf("mmap test start, pid: %d\n", pid);
	size_t SIZE = 1024 * 1024;
	if (argv >= 2)
	{
		SIZE *= atoi(argc[1]);
	}
	printf("press enter to start mmap sys\n");
	/*struct mmap_node head = {
		.addr = NULL,
		.length = 0,
	};*/
	struct mmap_node *head = (struct mmap_node*)malloc(sizeof(struct mmap_node));
	head->addr = NULL;
	head->length = 0;
	head->next = head;
	head->prev = head;
	char dummy;
	dummy = getchar();
	//fill_poll(SIZE, head, prepared_num);
	
	//gettimeofday(&start, NULL);
	// try to get memory from the poll
	char cmd[80];
	while (1)
	{
		char s[10];
		int new_size;
		int base = 1024;
		printf("type command\n");
		scanf("%s", cmd);
		if (strcmp(cmd, "1") == 0)
		{
			int populate = 1;
			printf("please specify new length in MB\n");
			scanf("%s", s);
			printf("please specify should populate. 0:no, 1:yes\n");
			scanf("%d", &populate);
			new_size = atoi(s);
			if (new_size > 0)
			{
				new_size *= base;
			}
			else
			{
				new_size = base;
			}
			// get mmap memory
			gettimeofday(&start, NULL);
			struct mmap_node *chunk = mmap_buf(new_size, head, populate);
			gettimeofday(&end, NULL);
			printf("got malloc memory, addr %ld\n", (unsigned long)chunk->addr);
			printf("execution time %ld us\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));

			// fill the memory
			printf("press enter to fill the memory\n");
			while((dummy = getchar()) != EOF && dummy != '\n');
			dummy = getchar();
			gettimeofday(&start, NULL);
			fill_chunk(chunk->addr, chunk->length);
			gettimeofday(&end, NULL);
			printf("fill time %ld us\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
			
			// release the memory
			printf("press enter to release the memory\n");
			//while((dummy = getchar()) != EOF && dummy != '\n');
			dummy = getchar();
			int errno;
			if (chunk->addr != NULL)
			{
				free(chunk->addr);
				free(chunk);
			}
		}
		else if (strcmp(cmd, "2") == 0)
		{
			printf("please specify new length\n");
			scanf("%s", s);
			int new_size = atoi(s);
			if (new_size > 0)
			{
				new_size *= base;
			}
			else
			{
				new_size = base;
			}
			fill_poll(new_size, head, prepared_num);
		} 
		else if (strcmp(cmd, "3") == 0)
		{
			break;
		}
		else if (strcmp(cmd, "4") == 0)
		{
			show_all_mmap(head);
		}
    else if (strcmp(cmd, "5") == 0)
    {
      alloc_small();
    }
    else if (strcmp(cmd, "6") == 0)
    {
      alloc_large();
    }
		else {
			show_menu();
		}
		printf("\n");
	}
	return 0;
}
