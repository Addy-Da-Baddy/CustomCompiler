#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 128

typedef struct HashmapEntry {
    char* key;
    int value;
    struct HashmapEntry* next;
} HashmapEntry;

typedef struct {
    HashmapEntry* buckets[HASHMAP_SIZE];
} Hashmap;

unsigned int hash(const char* key);
Hashmap* hashmap_create();
void hashmap_insert(Hashmap* map, const char* key, int value);
int hashmap_get(Hashmap* map, const char* key);
void hashmap_free(Hashmap* map);

#endif // HASHMAP_H