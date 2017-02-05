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
    hashtable_t *ht = malloc(sizeof(hashtable_t));
    ht->size = size;
    ht->buckets = calloc(sizeof(bucket_t *), size);
    return ht;
}

void ht_put(hashtable_t *ht, char *key, void *val) {
    /* FIXME: the current implementation doesn't update existing entries */
    unsigned int idx = hash(key) % ht->size;
    bucket_t *b = malloc(sizeof(bucket_t));


    // Iterate though list and check for same key. If key exists, update value
    int flag = 1;
    bucket_t *itr = NULL;
    for (itr = ht->buckets[idx]; itr->next != NULL; itr = itr->next) {
        if (itr->key == key) {
            itr->val = val;
            flag--;
            break;
        }
    }

    // If flag is true, the key doesn't exist, so make new bucket
    if (flag) {
        b->key = key;
        b->val = val;
        b->next = ht->buckets[idx];
        ht->buckets[idx] = b;
    }
}

void *ht_get(hashtable_t *ht, char *key) {
    unsigned int idx = hash(key) % ht->size;
    bucket_t *b = ht->buckets[idx];
    while (b) {
        if (strcmp(b->key, key) == 0) {
            return b->val;
        }
        b = b->next;
    }
    return NULL;
}

void ht_iter(hashtable_t *ht, int (*f)(char *, void *)) {
    bucket_t *b;
    unsigned long i;
    for (i = 0; i < ht->size; i++) {
        b = ht->buckets[i];
        while (b) {
            if (!f(b->key, b->val)) {
                return; // abort iteration
            }
            b = b->next;
        }
    }
}

void free_hashtable(hashtable_t *ht) {
    int i;
    for (i = 0; i < ht->size; i++) {
        bucket_t *curr;
        while ((curr = ht->buckets[i]) != NULL) {
            ht->buckets[i] = ht->buckets[i]->next;
            free(curr->key);
            free(curr->val);
        }

        free(curr);

        free(ht->buckets[i]);
    }

    free(ht); // FIXME: must free all substructures!
}

/* TODO */
void ht_del(hashtable_t *ht, char *key) {

    unsigned int idx = hash(key) % ht->size;
    bucket_t *prev = NULL, *itr;

    // Iterate through list
    for (itr = ht->buckets[idx]; itr->next != NULL; prev = itr, itr = itr->next) {
        if (itr->key == key) {
            // If clause for first element
            if (prev == NULL) {
                ht->buckets[idx] = itr->next;
                free(itr);
            } else {
                prev->next = itr->next; //Point previous element to next element
                free(itr);
            }
            break;
        }
    }

}

void ht_rehash(hashtable_t *ht, unsigned long newsize) {
}
