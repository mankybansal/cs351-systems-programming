#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

/* Daniel J. Bernstein's "times 33" string hash function, from comp.lang.C;
   See https://groups.google.com/forum/#!topic/comp.lang.c/lSKWXiuNOAk */
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

hashtable_t *make_hashtable(unsigned long size) {
  return NULL;
}

void ht_put(hashtable_t *ht, char *key, void *val) {
}

void *ht_get(hashtable_t *ht, char *key) {
  return NULL;
}

void ht_del(hashtable_t *ht, char *key) {
}

void ht_iter(hashtable_t *ht, int (*f)(char *, void *)) {
}

void ht_rehash(hashtable_t *ht, unsigned long newsize) {
}

void free_hashtable(hashtable_t *ht) {
}
