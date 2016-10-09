#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

#include "hashtable.h"

#define DEFAULT_CAPACITY 100
#define RESIZE_THRESHOLD 0.6
#define RESIZE_SCALING_FACTOR 10

static unsigned hash(char *key) {
   int len = strlen(key);
   unsigned hash = 0;
   while (len-- > 0) {
      hash += (unsigned) floor(pow(key[len], len));
   }
   return hash;
}

static unsigned double_hash(char *key) {
   int len = strlen(key);
   unsigned hash = 0;
   while (len-- > 0) {
      hash += (unsigned) floor(pow(key[len], len + 1));
   }
   return hash;
}

static void free_hashentry(struct hashentry *entry) {
   free(entry->key);
   free(entry->val);
   free(entry);
}

static void expand_table(struct hashtable *table) {
   unsigned old_capacity = table->capacity;
   struct hashentry **old_entries = *table->entries;
   struct hashentry *entry = **table->entries;
   size_t new_entry_space;
   int entry_index;
   table->capacity *= RESIZE_SCALING_FACTOR;
   new_entry_space = table->capacity * sizeof(struct hashentry *);
   *table->entries = malloc(new_entry_space);
   memset(*table->entries, 0, new_entry_space);
   for (entry_index = 0; entry_index < old_capacity; entry_index++) {
      if (entry->key != NULL) {
         set(table, entry->key, entry->val, entry->val_size);
         free_hashentry(entry);
      }
      entry += 1;
   }
   free(old_entries);
}

static struct hashentry *check_valid_entry(struct hashtable *table,
   char *key, unsigned hash) {
   if (table->size > RESIZE_THRESHOLD * table->capacity) {
      expand_table(table);
   }
   if ((*table->entries)[hash] == NULL) {
      (*table->entries)[hash] = malloc(sizeof(struct hashentry));
      memset((*table->entries)[hash], 0, sizeof(struct hashentry));
      table->size += 1;
      return (*table->entries)[hash];
   }
   if (strcmp((*table->entries)[hash]->key, key) == 0) {
      return (*table->entries)[hash];
   }
   hash = (hash + double_hash(key)) % table->capacity;
   return check_valid_entry(table, key, hash);
}

static struct hashentry *find_entry(struct hashtable *table, char *key) {
   unsigned hash_val;
   struct hashentry *entry;
   hash_val = hash(key);
   hash_val %= table->capacity;
   entry = check_valid_entry(table, key, hash_val);
   return entry;
}

struct hashtable *create_hashtable() {
   struct hashtable *table = malloc(sizeof(struct hashtable));
   size_t entry_space = DEFAULT_CAPACITY * sizeof(struct hashentry *);
   table->size = 0;
   table->capacity = DEFAULT_CAPACITY;
   table->entries = malloc(sizeof(struct hashentry **));
   *table->entries = malloc(entry_space);
   memset(*table->entries, 0, entry_space);
   return table;
}

void set(struct hashtable *table, char *key, void *val, size_t val_size) {
   struct hashentry *found = find_entry(table, key);
   if (found->key == NULL) {
      found->key = malloc((strlen(key) + 1) * sizeof(char));
      strcpy(found->key, key);
   }
   if (found->val != NULL) {
      free(found->val);
   }
   found->val = malloc(val_size);
   memcpy(found->val, val, val_size);
   found->val_size = val_size;
}

void *get(struct hashtable *table, char *key) {
   struct hashentry *found = find_entry(table, key);
   if (found->key == NULL) {
      table->size -= 1;
      free(found);
      return NULL;
   }
   return found->val;
}

void free_hashtable(struct hashtable *table) {
   struct hashentry *current;
   while (table->capacity-- > 0) {
      current = (*table->entries)[table->capacity];
      if (current != NULL) {
         free_hashentry(current);
      }
   }
   free(*table->entries);
   free(table->entries);
   free(table);
}
