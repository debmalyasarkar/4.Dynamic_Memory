/* Program to study mallopt() and parameter M_MMAP_THRESHOLD */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

//#define THRESHOLD_EN

#define SIZE 		130000
#define THRESHOLD_SIZE 	10000

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
  void *ptr = NULL, *ptr1 = NULL;

  printf("/* Before Malloc Call */\n");
  display_mallinfo();

#ifdef THRESHOLD_EN
  if(1 != mallopt(M_MMAP_THRESHOLD, THRESHOLD_SIZE))
  {  
    printf("mallopt failed\n");
    return -1;
  }
#endif

  ptr = (char *)malloc(SIZE);
  printf("/* After 1st Malloc Call */\n");
  display_mallinfo();

  ptr1 = (char *)malloc(SIZE);
  printf("/* After 2st Malloc Call */\n");
  display_mallinfo();

  free(ptr);

  free(ptr1);

  return 0;
}

#if 0
Note :
1. First disable the THRESHOLD_EN option by commenting it out.
   Run the program.
   Observe that the consecutive calls to malloc causes the 135K region to get
   extended and form a larger region in heap.
   No MMAP region is used here.
2. Now enable the THRESHOLD_EN option and run the program again.
   Observe that the first call uses the heap.
   The second call to malloc causes it to allocate the memory in the MMAP
   region.
   This is because the M_MMAP_THRESHOLD tells the malloc() to allocate the
   memory (greater or equal to M_MMAP_THRESHOLD) in MMAP if it isnt available
   from the heap pool.
#endif
