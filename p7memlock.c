#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#define USE_POSIX_MEMALIGN
//#define USE_MMAP

#define MAX_PAGE_NO   10
#define LOCK_PAGE_NO  4

int main(void)
{
  unsigned char mincore_vect[MAX_PAGE_NO];
  size_t page_size, page_index, lock_size, size;
  void *ptr = NULL;

  /* Find page size */
  page_size = getpagesize();
  printf("Page Size = %u\n",page_size);

  /* Calculate total amount of memory to be allocated */
  size = page_size * MAX_PAGE_NO;
  printf("Total No of Pages Allocated = %u\n",MAX_PAGE_NO);

  /* Calculate total amount of memory to be locked */
  lock_size = page_size * LOCK_PAGE_NO;
  printf("Total No of Pages to Lock   = %u\n",LOCK_PAGE_NO);

#ifdef  USE_POSIX_MEMALIGN
#ifndef USE_MMAP
  /* Allocate the memory aligned to page size */
  /* This is an alternative way to allocate memory than malloc() */
  /* The reference to the allocated memory is used in subsequent functions */
  if(0 != posix_memalign(&ptr, page_size, size))
  {  
    perror("posix_memalign : ");
    return -1;
  }
#endif
#endif

#ifdef  USE_MMAP
#ifndef USE_POSIX_MEMALIGN
  /* Create a mapping into the virtual address space for the caller process */
  /* The reference to the allocated memory is used in subsequent functions */
  ptr = mmap(NULL, size, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED)
  {
    perror("mmap  :");
    return -1;
  }
#endif
#endif

  /* Lock the specified number of pages in RAM so they are not swapped out */
  if(0 != mlock(ptr, lock_size))
  {  
    perror("mlock : ");
    return -1;
  }

  /* Determine whether pages are resident in memory */
  if(0 != mincore(ptr, size, mincore_vect))
  {
    perror("mincore : ");
    return -1;
  }

  /* The least significant bit of each byte of the vector array  will be set 
     if the corresponding page is currently resident/active in memory, and be
     clear otherwise */
  for(page_index = 0; page_index < MAX_PAGE_NO; page_index++)
  {
    if(mincore_vect[page_index] & 1)
      printf("Active : %u\n",page_index);
  }
  return 0;
}

#if 0
Note :
1. We can use either USE_POSIX_MEMALIGN or USE_MMAP.
   Enable any one Macro and Disable the Other One.
   If both options are enabled or both are disabled then automatically none of
   the sections are compiled and the code fails with an error on mlock()
2. Set MAX_PAGE_NO = 10 and LOCK_PAGE_NO = 0 and build the program to run it.
   We observe that as no pages are locked, even though the memory is allocated
   still none of the pages are active in memory.
3. Set LOCK_PAGE_NO to a positive value less than or equal to MAX_PAGE_NO.
   Build the code and run it again.
   We observe that now few pages are active in memory.This is because we have 
   locked those pages in RAM so they are reported as active. 
   The mlock function is used to lock the pages in memory and to prevent them
   from being swapped out.
   If we call the munlock function to make these pages swap capable again, we
   get pages as active because once they are in RAM, they are already active.
   The munlock call does not remove them from memory, it simply assigns these 
   pages to be swap capable. They will be swapped if kernel deems it necessary.
#endif

