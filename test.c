#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
  int chunk_size = 1024 * 256;
  int chunk_num = 1024 / 256;
  struct timeval start;
  struct timeval end;
  char* chunks[chunk_num];
  gettimeofday(&start, NULL);
  for (int i = 0; i < chunk_num; i++)
  {
    chunks[i] = (char*)malloc(chunk_size);
    
    for (int j = 0; j < chunk_size; j++)
    {
      chunks[i][j] = 'a';
    }
    
    //mlock(chunks[i], chunk_size);
  }
  gettimeofday(&end, NULL);
  printf("fill time: %ld us\n", (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec));
  return 0;

}
