/* Program to study dynamic memory allocation using malloc_stats() */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define SIZE 100

int main()
{
  void *ptr = NULL;
  
  printf("/* Before Malloc Call */\n");
  malloc_stats();
  printf("\n\n");
  
  printf("/* After Malloc Call */\n");
  ptr = (char *) malloc(SIZE);
  malloc_stats();
  printf("\n\n");
  
  printf("Pointer    	 = %p\n",ptr);
  printf("Usable Size 	 = %d\n\n",malloc_usable_size(ptr));

  printf("/* After Free Call */\n");
  free(ptr);
  malloc_stats();
  printf("\n\n");
  return 0;
}

#if 0
Notes :

1. Alignment policy causes more bytes to be allocated than specified size.
2. Initially some system bytes may or may not be used based on setup.
3. Based on the setup, some functions like getchar() may cause heap to be 
   used. This is reflected in the system bytes.
4. Allocation causes 135K chunk to be allocated on first call to malloc.
5. Consecutive calls to malloc does not allocate 135K repeatedly, instead the 
   memory is allocated in the same pool of 135K.
6. After calling free the allocated memory is returned back to the free pool.
7. For consecutive allocations it is observed that the contiguous pointer 
   addresses are returned.
   [Not guaranteed by C Standard & maybe implementation dependent]
8. After consecutive allocations when free is called,the memory is returned 
   back to the pool.
   The order of calling free for each pointer is insignificant here i.e the
   free call to pointers may not be in the same order as allocation.
9. If we allocate once and again free, then again reallocate then the pointer
   is found to be same.[Not guaranteed by C Standard]
10.Size greater than 134116 bytes or 130K as per MMAP_THRESHOLD are allocated
   in the mmap region.
11.Allocating a size greater than 135K repeatedly causes the mmap bytes to get
   incremented as seen in malloc_stats.
   On calling free the total bytes used get decremented i.e the mmap gets 
   freed but these mmap byte counters maintain the history to indicate the 
   number of allocations made.
12.Point numbers 7 and 9 are applicable only for heap allocations.
   They are not applicable for mmap allocations.
13.Consecutive calls to malloc where the second size cannot be allocated in the
   first chunk causes the first chunk to expand and create a larger chunk.
   Eg. Two calls of 130K causes heap to have a single chunk of ~226K.
14.The above behaviour can be controlled by mallopt options M_MMAP_THRESHOLD.
15.The 135K limit can also be controlled by the M_TOP_PAD option in mallopt.
#endif
