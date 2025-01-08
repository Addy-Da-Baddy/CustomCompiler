#include "hashmap.h"
#include <stdlib.h>
#include <string.h>

#define HASHMAP_SIZE 128

typedef struct HashMapEntry {
    char* key;
    int value;
    struct HashMapEntry* next;
} HashMapEntry;

struct HashMap {
    HashMapEntry* buckets[HASHMAP_SIZE];
};

// Simple hash function
static unsigned int hash(const char* key) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = 31 * hash + key[i];
    }
    return hash % HASHMAP_SIZE;
}

HashMap* hashmap_create() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        map->buckets[i] = NULL;
    }
    return map;
}

void hashmap_free(HashMap* map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        HashMapEntry* entry = map->buckets[i];
        while (entry != NULL) {
            HashMapEntry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(map);
}

int hashmap_put(HashMap* map, const char* key, int value) {
    unsigned int index = hash(key);
    HashMapEntry* entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return 0;
        }
        entry = entry->next;
    }

    // Create a new entry
    entry = (HashMapEntry*)malloc(sizeof(HashMapEntry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = map->buckets[index];
    map->buckets[index] = entry;
    return 0;
}

int hashmap_get(HashMap* map, const char* key) {
    unsigned int index = hash(key);
    HashMapEntry* entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    return -1; // Not found
}