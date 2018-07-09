#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main(void)
{
  int ii;
  void *ptr = NULL;
  
  ptr = (char *)malloc(SIZE);

  for(ii = 0; ii < SIZE; ii++)
    *(char *)(ptr + ii) = ii;

  for(ii = 0; ii < SIZE; ii++)
    printf("%d ",*(char *)(ptr + ii));
  printf("\n");

  free(ptr);
  
  for(ii = 0; ii < SIZE; ii++)
    printf("%d ",*(char *)(ptr + ii));
  printf("\n");
  return 0;
}

#if 0
Based on the execution environment the sizeof(int) would be 4 or 8 bytes.
After the free operation the bytes corresponding to the sizeof(int) on that
system are modified (Cleared by writing 0 or Some Garbage Data).
The others bytes are observed to be intact.
This may be due to the implementation of free function which may clear the 
first int on being called.

This behaviour of getting values by dereferencng a pointer after free() is
set to be undefined behaviour according to C Standard.
#endif
