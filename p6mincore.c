#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

//#define ACCESS_MEMORY

#define MAX 5

int main(void)
{
  unsigned char mincore_vect[MAX];
  size_t page_size, page_index, size;
  void *ptr;

  /* Find page size */
  page_size = getpagesize();
  printf("Page Size = %u\n",page_size);

  /* Calculate total amount of memory to be allocated */
  size = page_size * MAX;
  printf("Total Allocated Size = %u\n",size);

  /* Allocate the memory aligned to page size */
  /* This is an alternative way to allocate memory than malloc() */
  /* The reference to the allocated memory is used in subsequent functions */
  if(0 != posix_memalign(&ptr, page_size, size))
    perror("posix_memalign : ");

#ifdef ACCESS_MEMORY
  /* Renders the allocated memory as active by accessing it for writing */
  memset(ptr,0,size);
#endif

  /* Determine whether pages are resident in memory */
  if(0 != mincore(ptr, size, mincore_vect))
    perror("mincore : ");

  /* The least significant bit of each byte of the vector array  will be set 
     if the corresponding page is currently resident/active in memory, and be
     clear otherwise */
  for(page_index = 0; page_index < MAX; page_index++)
  {
    if(mincore_vect[page_index] & 1)
      printf("Active : %u\n",page_index);
  }
  return 0;
}

#if 0
Note :
1. Switch OFF the ACCESS_MEMORY macro by commenting it out and run the program.
   We observe that though memory is allocated but the memory is not actually 
   active or resident in physical RAM.
2. Switch ON the ACCESS_MEMORY macro by removing comment from it and run the 
   program.
   We observe that now the pages are active in memory.
#endif
