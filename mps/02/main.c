#include <stdio.h>
#include <stdlib.h>
#include "sudoku.h"

int main(int argc, char *argv[]) {
  int n=0, row;
  char buf[80];
  unsigned char vals[9][9];
  while (!feof(stdin)) {
    if (!fgets(buf, sizeof(buf), stdin)) { // throw away puzzle heading
      break; // in case EOF after previous newline
    }
    for (row=0; row<9; row++) {
      fread(vals[row], 1, 9, stdin);
      fgets(buf, sizeof(buf), stdin); // throw away rest of line
    }
    printf("Solving puzzle %d\n", ++n);
    solve(vals);
  }
  return 0;
}
