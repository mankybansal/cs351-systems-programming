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

void print_ht_stats(hashtable_t *ht) {
  bucket_t *b;
  unsigned long idx, len, max_len=0, num_buckets=0, num_chains=0;
  for (idx=0; idx<ht->size; idx++) {
    b = ht->buckets[idx];
    len = 0;
    while (b) {
      len++;
      num_buckets++;
      b = b->next;
    }
    if (len > 0) {
      num_chains++;
    }
    if (max_len < len) {
      max_len = len;
    }
  }
  printf("Num buckets = %lu\n", num_buckets);
  printf("Max chain length = %lu\n", max_len);
  printf("Avg chain length = %0.2f\n", (float)num_buckets / num_chains);
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
      print_ht_stats(ht);
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
