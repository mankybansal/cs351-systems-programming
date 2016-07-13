#ifndef HASHTABLE_T
#define HASHTABLE_T

typedef struct hashtable hashtable_t;
typedef struct bucket bucket_t;

struct bucket {
  char *key;
  void *val;
  bucket_t *next;
};

struct hashtable {
  unsigned long size;
  bucket_t **buckets;
};

unsigned long hash(char *str);

hashtable_t *make_hashtable(unsigned long size);
void  ht_put(hashtable_t *ht, char *key, void *val);
void *ht_get(hashtable_t *ht, char *key);
void  ht_del(hashtable_t *ht, char *key);
void  ht_iter(hashtable_t *ht, int (*f)(char *, void *));
void  ht_rehash(hashtable_t *ht, unsigned long newsize);
void  free_hashtable(hashtable_t *ht);

#endif
