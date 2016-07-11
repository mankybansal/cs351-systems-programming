#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "sudoku.h"

int main(int argc, char *argv[]) {
  int npuz=0, nval, c;
  int has_headers = 0;
  char buf[80];
  unsigned char vals[9][9];

  if ((c = fgetc(stdin)) == 'G') {
    has_headers = 1;
  } else {
    ungetc(c, stdin);
  }

  while (!feof(stdin)) {
    if (has_headers) {
      fgets(buf, sizeof(buf), stdin);
    }

    nval = 0;
    do {
      c = fgetc(stdin);
      if (isdigit(c) || c == '.') {
        *((char *)vals + nval++) = c;
      } else if (c == EOF) {
        goto done;
      }
    } while (nval<(9*9));

    fgets(buf, sizeof(buf), stdin);

    printf("Solving puzzle %d\n", ++npuz);
    solve(vals);
  }
  
 done:
  return 0;
}
