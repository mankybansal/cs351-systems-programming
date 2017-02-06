#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

    unsigned int idx = hash(key) % ht->size;
    bucket_t *b = malloc(sizeof(bucket_t));

    b->key = key;
    b->val = val;
    b->next = ht->buckets[idx];

    bucket_t *itr = ht->buckets[idx];
    int flag = 1;

    for (; itr != NULL; itr = itr->next)
        if (strcmp(itr->key, key) == 0) {
            free(itr->val);
            itr->val = val;
            flag--;
            break;
        }

    if (flag) ht->buckets[idx] = b, free(itr);
    else free(b);

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
    bucket_t *b;
    unsigned long i;
    for (i = 0; i < ht->size; i++) {
        b = ht->buckets[i];
        while (b) {
            bucket_t *c = b;
            b = b->next;
            ht_del(ht, c->key);
        }
    }
    free(b);
    free(ht->buckets);
}

void ht_del(hashtable_t *ht, char *key) {

    unsigned int idx = hash(key) % ht->size;
    bucket_t *prev = NULL, *itr = ht->buckets[idx];

    // Iterate through list
    for (; itr != NULL; prev = itr, itr = itr->next)
        if (strcmp(itr->key, key) == 0) {
            // If clause for first element
            if (prev == NULL)
                ht->buckets[idx] = itr->next;
            else
                prev->next = itr->next;

            free(itr->key);
            free(itr->val);
            free(itr);
            break;
        }


    if (ht->buckets[idx] == NULL){
        free(ht->buckets[idx]);
        free(prev);
    }
}

void ht_rehash(hashtable_t *ht, unsigned long newsize) {

    hashtable_t *new_ht = make_hashtable(newsize);

    unsigned long i;
    for (i = 0; i < ht->size; i++) {
        bucket_t *curr = ht->buckets[i];
        while (curr) {
            char *c = strdup(curr->key);
            void *s = strdup(curr->val);

            ht_put(new_ht, c, s);
            curr = curr->next;
        }
    }

    free_hashtable(ht);
    *ht = *new_ht;
    free(new_ht);
}
