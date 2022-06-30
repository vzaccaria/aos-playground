#include <stdio.h>
#include <stdlib.h>

/* Compile with -fno-pie */

int main(int argc, char *argv[]) {
  printf("location of code : %p\n", main);
  printf("press a key..\n");
  int x = 0;
  while (getchar() != 'q') {
    printf("value of x @%p = %d\n", &x, x);
    x = x + 1;
  }
  return 0;
}
