#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result" 

int print_iter(char *key, void *val) {
  printf("%s -> %s\n", key, (char *)val);
  return 1;
}

void eval_tracefile(char *filename) {
  FILE *infile;
  int ht_size;
  char buf[80], *key, *val;
  hashtable_t *ht;

  if ((infile = fopen(filename, "r")) == NULL) {
    printf("Error opening tracefile %s\n", filename);
    exit(1);
  }

  fscanf(infile, "%d", &ht_size);
  printf("Creating hashtable of size %d\n", ht_size);
  ht = make_hashtable(ht_size);

  while (fscanf(infile, "%s", buf) != EOF) {
    switch(buf[0]) {
    case 'p':
      fscanf(infile, "%s", buf);
      key = strdup(buf);
      fscanf(infile, "%s", buf);
      val = strdup(buf);
      printf("Inserting %s => %s\n", key, val);
      ht_put(ht, key, val);
      break;
    case 'g':
      fscanf(infile, "%s", buf);
      printf("Looking up key %s\n", buf);
      if ((val = ht_get(ht, buf))) {
        printf("Found value %s\n", val);
      } else {
        printf("Key not found\n");
      }
      break;
    case 'd':
      fscanf(infile, "%s", buf);
      printf("Removing key %s\n", buf);
      ht_del(ht, buf);
      break;
    case 'r':
      fscanf(infile, "%d", &ht_size);
      printf("Rehashing to %d buckets\n", ht_size);
      ht_rehash(ht, ht_size);
      break;
    case 'i':
      printf("Printing hashtable info\n");
      ht_iter(ht, print_iter);
      printf("Max chain length = %d\n", ht_max_chain_length(ht));
      break;
    default:
      printf("Bad tracefile directive (%c)", buf[0]);
      exit(1);
    }
  }
  free_hashtable(ht);
  fclose(infile);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s TRACEFILE_NAME\n", argv[0]);
    exit(0);
  }
  eval_tracefile(argv[1]);
  return 0;
}

#pragma GCC diagnostic pop
