
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main () {
  FILE *f;
  size_t i, j;
  char buf[4096];
  
  f = fopen("task1.tsko", "rb");
  i = 0;
  while (!feof(f)) {
    i += fread(buf + i, 1, 1, f);
  }
  buf[i] = 0;
  fclose(f);
  
  f = fopen("task1.tsko", "wb");
  fprintf(f, "%c", 0xe9);
  fprintf(f, "%d", (int) (4096 - i));
  for (j = 5; j < 4096 - i; j++) {
    fprintf(f, "%c", 0xe5);
  }
  fwrite(buf, 1, i, f);
  
  fclose(f);
  return 0;
}
