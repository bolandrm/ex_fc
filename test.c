#include <stdint.h>
#include <stdio.h>

uint32_t start = 4294967294 - 2000;
uint32_t end = 50;

void main(void) {
  printf("ok.. %u \n", end-start);
}
