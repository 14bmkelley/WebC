#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <sys/types.h>

struct hashentry {
   char *key;
   void *val;
   size_t val_size;
};

struct hashtable {
   struct hashentry ***entries;
   unsigned size, capacity;
};

struct hashtable *create_hashtable();
void set(struct hashtable *, char *, void *, size_t);
void *get(struct hashtable *, char *);
void free_hashtable(struct hashtable *);

#endif
