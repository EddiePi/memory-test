#include <iostream>
#include <ctime>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

using namespace std;

//#define TOTAL_SIZE 2097152
#define TOTAL_SIZE 5242880

//#define TOTAL_SIZE 419430400 // 400M

long time_period(timeval s, timeval e)
{
  return (e.tv_sec - s.tv_sec) * 1000000 + (e.tv_usec - s.tv_usec);
}

int main()
{
  pid_t pid = getpid();
  printf("pid: %d\n", pid);
  size_t chunk_size;
  size_t alloc_size;
  void *cur_top;
  printf("set this process as pri process and press enter to start\n");
  getchar();
  
  // metrics
  timeval ts, te;
  for (chunk_size = 32768; chunk_size < 2097154; chunk_size *= 2)
  {
    alloc_size = 0;
    gettimeofday(&ts, NULL);
    int i = 0;
    int count = 200;
    while (alloc_size < TOTAL_SIZE)
    {
      cur_top = (sbrk(chunk_size));
      if (mlock(cur_top, chunk_size) != 0)
      {
        printf("mlock failed. %s\n", strerror(errno));
      }
      
      alloc_size += chunk_size;
    }
    gettimeofday(&te, NULL);
    printf("chunk: %ld, time: %ld us\n", chunk_size, time_period(ts, te));
    printf("press enter to release memory\n");
    getchar();
    sbrk(-alloc_size);
    printf("press enter to start next round\n");
    getchar();
  }
  return 0;
}
