#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sched.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/syscall.h>
#include <asm/ldt.h>

#define size2pool_index(s)  \
  ({  \
    int i = 0;  \
    int sz = s >> 18; \
    while (sz > 0) {  \
      i++;  \
      sz >>= 1; \
    } \
    if (i > 15) { \
      i = 15; \
    } \
    i;  \
   })

#define INTERNAL_SIZE_T size_t
#define SIZE_SZ                (sizeof(INTERNAL_SIZE_T))
#define MALLOC_ALIGNMENT       (2 *SIZE_SZ)
#define MALLOC_ALIGN_MASK      (MALLOC_ALIGNMENT - 1)
#define STACK_SIZE 65536

#define chunk2mem(p)   ((void*)((char*)(p) + 2*SIZE_SZ))

int mtx = 0;
static char thread_stack[STACK_SIZE];
int test_mutex_val = 0;

static long sys_futex(void *addr1, int op, int val1, struct timespec *timeout, void *addr2, int val3)
{
  return syscall(SYS_futex, addr1, op, val1, timeout, addr2, val3);
}

int mutex_lock(int *m)
{
  int c;
  //c = atomic_compare_exchange_strong(m, 0, 1);
  c = *m;
  if (*m == 0)
  {
    *m = 1;
  }
  if (!c)
    return 0;

  if (c == 1)
    *m = 2;
  while (c)
  {
    sys_futex(m, FUTEX_WAIT_PRIVATE, 2, NULL, NULL, 0);
    c = *m;
    *m = 2;
  }
  return 0;
}

int mutex_unlock(int *m)
{
  if (*m == 2)
  {
    *m = 0;
  }
  else if (*m == 1)
  {
    *m = 0;
    return 0;
  }
  if (*m)
  {
    if (*m == 1)
    {
      *m = 2;
      return 0;
    }
  }
  sys_futex(m, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);

  return 0;
}

int test_thread(void* arg)
{
    printf("thread started\n");
    printf("waiting on val\n");
    //long f_ret = syscall(SYS_futex, &test_mutex_val, FUTEX_WAIT_PRIVATE, 0, NULL, NULL, 0);
    mutex_lock(&mtx);
    printf("got val\n");
    mutex_unlock(&mtx);
    return 0;
}

struct malloc_chunk
{
    INTERNAL_SIZE_T size;
    INTERNAL_SIZE_T prev_size;
};

struct malloc_chunk_wrap
{
    struct malloc_chunk chunk_ptr;
    struct malloc_chunk_wrap* prev;
    struct malloc_chunk_wrap* next;
};

struct mmap_pool 
{
    struct malloc_chunk_wrap pool[16];
};

static struct mmap_pool m_pool;

int main() {
    int thread_ret;
    printf("creating management thread\n");
    mutex_lock(&mtx);
    const int clone_flags = (CLONE_VM | CLONE_SYSVSEM | CLONE_FS | CLONE_FILES
          | 0);
    pid_t tid = syscall(SYS_gettid);
    struct user_desc* u_info = (struct user_desc*)malloc(sizeof(struct user_desc));
    u_info->entry_number = -1;
    syscall(SYS_set_thread_area, u_info);
    thread_ret = clone(&test_thread, 
          (void*)thread_stack + STACK_SIZE,
          clone_flags, NULL, &tid, u_info, &tid);
    if (thread_ret == -1)
    { 
        printf("fail to create mkanagement thread, %s\n", strerror(errno));
    }
    else
    { 
        printf("success to create auxiliary thread\n");
    }
    char c = getchar();
    printf("got char: %c\n", c);
    //futex(&test_mutex_val, FUTEX_WAIT, 1, NULL, NULL, 0);
    //syscall(SYS_futex, &test_mutex_val, FUTEX_WAKE_PRIVATE, 1, NULL, NULL, 0);
    mutex_unlock(&mtx);
    printf("wake sent\n");
    c = getchar();

    printf("exiting main\n");
    return 0;
}
