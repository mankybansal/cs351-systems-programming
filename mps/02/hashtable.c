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

    /* FIXME: the current implementation doesn't update existing entries */
    unsigned int idx = hash(key) % ht->size;
    bucket_t *b = malloc(sizeof(bucket_t));

    b->key = key;
    b->val = val;
    b->next = ht->buckets[idx];

    bucket_t *itr = ht->buckets[idx];
    int flag = 1;

    for (; itr != NULL; itr = itr->next) {
        //printf("\n\nFOUND\n\n");
        if (strcmp(itr->key, key) == 0) {
            itr->val = val;
            flag--;
            break;
        }
    }


    if (flag) {
        ht->buckets[idx] = b;
    } else {
        free(b);
    }

    //print_ht(ht);

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

/*
void print_ht(hashtable_t *ht) {


    printf("\n------HASHTABLE--------\n");
    bucket_t *b;
    unsigned long i;
    for (i = 0; i < ht->size; i++) {
        b = ht->buckets[i];
        while (b) {
            printf("  %s->%s  ", b->key, b->val);
            b = b->next;
        }
        printf("\n");
    }

    printf("\n----------------------\n");
}*/

void free_hashtable(hashtable_t *ht) {

    unsigned long i;
    for (i = 0; i < ht->size; i++) {
        bucket_t *curr = ht->buckets[i];
        while (curr) {
            free(curr->key);
            free(curr->val);
            curr = curr->next;
        }
        free(curr);
        free(ht->buckets[i]);
    }

    //free(ht); // FIXME: must free all substructures!
}

/* TODO */
void ht_del(hashtable_t *ht, char *key) {

    //print_ht(ht);
    unsigned int idx = hash(key) % ht->size;
    bucket_t *prev = NULL, *itr = ht->buckets[idx];

    // Iterate through list
    for (; itr != NULL; prev = itr, itr = itr->next) {
        if (strcmp(itr->key,key)==0) {
            // If clause for first element
            if (prev == NULL) {
                ht->buckets[idx] = itr->next;
            } else {
                prev->next = itr->next; //Point previous element to next element
            }
            break;
        }
    }

    free(itr);

}


/*

void print_ht_stats2(hashtable_t *ht) {
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
    printf("\n\n\n\n");
}
*/

void ht_rehash(hashtable_t *ht, unsigned long newsize) {

    hashtable_t *newht = malloc(sizeof(hashtable_t));
    newht->size = newsize;
    newht->buckets = calloc(sizeof(bucket_t *), newsize);

    unsigned long i;
    for (i = 0; i < ht->size; i++) {
        bucket_t *curr = ht->buckets[i];
        while (curr) {
            char *c = strdup(curr->key);
            void *s = strdup(curr->val);
            ht_put(newht, c, s);

            curr = curr->next;
        }
    }


    free_hashtable(ht);

    *ht = *newht;
    //print_ht(ht);
    //print_ht_stats2(ht);

}
