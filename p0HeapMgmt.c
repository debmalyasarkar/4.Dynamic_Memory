/* Program to study the usage of brk and sbrk for heap management */
#include <stdio.h>
#include <unistd.h>


void *cur_brk, *def_brk, *new_brk;

int main(void)
{
  /* Get default location of heap */
  cur_brk = sbrk(0);
  def_brk = cur_brk;
  printf("%p\n",cur_brk);
  
  /* Allocate 100 bytes in heap */
  brk(cur_brk + 100);

  /* Verify if the bytes are allocated by checking current position */
  new_brk = sbrk(0);
  printf("%p\n",new_brk);

  /* Deallocate 100 bytes in heap */
  brk(def_brk);

  /* Verify if the bytes are deallocated by checking current position */
  cur_brk = sbrk(0);

  /* Note : The following line causes Segmentation Fault in some distributions.
            In other distributions it is found to be working.
  By using GDB we can check the position skipping this printf */
  printf("%p\n",cur_brk);

  return 0;
}
