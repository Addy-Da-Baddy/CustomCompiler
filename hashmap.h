#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

#define TABLE_SIZE 100

typedef struct Entry {
    char* key;
    int value;
    struct Entry* next;
} Entry;

typedef struct {
    Entry* table[TABLE_SIZE];
    int size;
} HashMap;

HashMap* hashmap_create();
void hashmap_put(HashMap* map, const char* key, int value);
int hashmap_get(HashMap* map, const char* key, int* value);
void hashmap_free(HashMap* map);

#endif // HASHMAP_H