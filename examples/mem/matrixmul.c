#include <stdlib.h>
#include <stdio.h>

#define DIM 500
#define KIJ

int main(int argc, char *argv[]) {
  volatile int A[DIM][DIM], B[DIM][DIM], C[DIM][DIM], r;
  int i, j, k, n, iters;

  if (argc < 2) {
    printf("Specify num iterations as argument\n");
    exit(1);
  } else {
    iters = atoi(argv[1]);
  }
  
  for (i=0; i<DIM*DIM; i++) {
    ((int *)A)[i] = (int)random();
    ((int *)B)[i] = (int)random();
  }

  for (n=0; n<iters; n++) {
#ifdef IJK
    for (i=0; i<DIM; i++) {
      for (j=0; j<DIM; j++) {
        C[i][j] = 0;
        for (k=0; k<DIM; k++) {
          C[i][j] += A[i][k] * B[k][j];
        }
      }
    }
#elif defined KJI
    for (k=0; k<DIM; k++) {
      for (j=0; j<DIM; j++) {
        r = B[k][j];
        for (i=0; i<DIM; i++)
          C[i][j] += A[i][k]*r;
      }
    }
#elif defined KIJ
    for (k=0; k<DIM; k++) {
      for (i=0; i<DIM; i++) {
        r = A[i][k];
        for (j=0; j<DIM; j++)
          C[i][j] += r*B[k][j];
      }
    }
#endif
  }

  return 0;
}
