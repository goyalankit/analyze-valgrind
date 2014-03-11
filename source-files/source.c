#include "/work/02681/ankitg/workspace/valgrind/gleipnir/gleipnir.h"
#include <stdlib.h>
#include <stdio.h>

#define n 900
double a[n][n], b[n][n], c[n][n];

void compute()
{
GL_GLOBAL_START_INSTR;
  register int i, j, k;
  for (i = 0; i < n; i++) {
      for (k = 0; k < n; k++) {
    for (j = 0; j < n; j++) {
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
GL_GLOBAL_STOP_INSTR;
}

int main(int argc, char *argv[])
{
  register int i, j, k;

  printf("varinfo:a:810000:%08lx:%lu\n",(void *) &a, sizeof(a[0][0]));
  printf("varinfo:b:810000:%08lx:%lu\n",(void *) &b, sizeof(b[0][0]));
  printf("varinfo:c:810000:%08lx:%lu\n",(void *) &c, sizeof(c[0][0]));

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      a[i][j] = i+j;
      b[i][j] = i-j;
      c[i][j] = 0;
    }
  }

  compute();

  printf("%.1lf\n", c[3][3]);

  return 0;
}
