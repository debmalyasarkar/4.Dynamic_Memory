/* Program to study mallinfo() and the mallinfo structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define SIZE 1024

void display_mallinfo(void)
{
  struct mallinfo mi;

  mi = mallinfo();

  printf("Total non-mmapped bytes     (arena)   :	%d\n", mi.arena);
  printf("No of free chunks           (ordblks) :	%d\n", mi.ordblks);
  printf("No of free fastbin blocks   (smblks)  :	%d\n", mi.smblks);
  printf("No of mapped regions        (hblks)   :	%d\n", mi.hblks);
  printf("Bytes in mapped regions     (hblkhd)  :	%d\n", mi.hblkhd);
  printf("Max. total allocated space  (usmblks) :	%d\n", mi.usmblks);
  printf("Free bytes held in fastbins (fsmblks) :	%d\n", mi.fsmblks);
  printf("Total allocated space       (uordblks):	%d\n", mi.uordblks);
  printf("Total free space            (fordblks):	%d\n", mi.fordblks);
  printf("Topmost releasable block    (keepcost):	%d\n", mi.keepcost);
  printf("\n\n");
}

int main(void)
{
  void *ptr = NULL;

  printf("/* Before Malloc Call */\n");
  display_mallinfo();

  ptr = (char *)malloc(SIZE);
  printf("/* After Malloc Call */\n");
  display_mallinfo();

  free(ptr);
  printf("/* After Free Call */\n");
  display_mallinfo();
  return 0;
}

#if 0
Note :
1. The fields of the mallinfo structure contain the following information:

   arena     The total amount of memory allocated by means other than mmap() 
             (i.e. memory  allocated  on  the  heap).
             This figure includes both in-use blocks and blocks on the free 
             list.

   ordblks   The number of ordinary (i.e., non-fastbin) free blocks.

   smblks    The number of fastbin free blocks.

   hblks     The number of blocks currently allocated using mmap().

   hblkhd    The number of bytes in blocks currently allocated using mmap().

   usmblks   The  "highwater  mark"  for  allocated  space—that is, the maximum
             amount of space that was ever allocated.  
             This field is maintained only in nonthreading environments.

   fsmblks   The total number of bytes in fastbin free blocks.

   uordblks  The total number of bytes used by in-use allocations.

   fordblks  The total number of bytes in free blocks.

   keepcost  The total amount of releasable free space at the top of the heap.  
             This is the maximum number of bytes that could ideally 
             (i.e. ignoring page alignment restrictions, and so on) be released
             by malloc_trim().
   
2. Here even when the memory is allocated in mmap and it is freed, the history
   is not retained unlike malloc_stats().
#endif
