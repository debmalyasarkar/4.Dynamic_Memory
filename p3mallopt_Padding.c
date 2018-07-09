/* Program to study mallopt() and memory allocation parameters M_TOP_PAD */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define PADDING_EN
//#define TRIMMING_EN

#define SIZE 		4096
#define PADDING_SIZE 	10000

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

#ifdef PADDING_EN
  if(1 != mallopt(M_TOP_PAD, PADDING_SIZE))
  {  
    printf("mallopt failed\n");
    return -1;
  }
#endif

  ptr = (char *)malloc(SIZE);
  printf("/* After Malloc Call */\n");
  display_mallinfo();

#ifdef TRIMMING_EN
  if(1 != malloc_trim(0))
  {
    printf("malloc_trim failed to release memory\n");
    return -1;
  }
#endif

  free(ptr);
  printf("/* After Free Call */\n");
  display_mallinfo();
  return 0;
}

#if 0
Note :

The topmost releasable block (keepcost) option in mallinfo is to be observed.

1. First Enable Only the PADDING_EN option and run the program.
   Disable the TRIMMING_EN option by commenting it out.
   The observation is that after free operation the PADDING_SIZE bytes are
   retained on the top of heap.
   The remaining memory is freed.
   Change the PADDING_SIZE and repeat the process.
2. Now Enable Only the TRIMMING_EN option and run the program.
   Disable the PADDING_EN option by commenting it out.
   The observation is that after free operation only minimum memory (multiple 
   of page size) is retained on top of heap.
3. Now Change the argument of malloc_trim to a value of your choice and run the
   program.
   The observation is that after free operation only that much memory (multiple
   of page size) is retained on top of heap.
#endif
