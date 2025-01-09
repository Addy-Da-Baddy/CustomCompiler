#include "hashmap.h"

unsigned int hash(const char* key) {
    unsigned int hashval = 0;
    for (; *key != '\0'; key++) {
        hashval = *key + (hashval << 5) - hashval;
    }
    return hashval % HASHMAP_SIZE;
}

Hashmap* hashmap_create() {
    Hashmap* map = (Hashmap*)malloc(sizeof(Hashmap));
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        map->buckets[i] = NULL;
    }
    return map;
}

void hashmap_insert(Hashmap* map, const char* key, int value) {
    unsigned int bucket = hash(key);
    HashmapEntry* entry = map->buckets[bucket];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    HashmapEntry* new_entry = (HashmapEntry*)malloc(sizeof(HashmapEntry));
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = map->buckets[bucket];
    map->buckets[bucket] = new_entry;
}

int hashmap_get(Hashmap* map, const char* key) {
    unsigned int bucket = hash(key);
    HashmapEntry* entry = map->buckets[bucket];

    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return -1; // Key not found
}

void hashmap_free(Hashmap* map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        HashmapEntry* entry = map->buckets[i];
        while (entry != NULL) {
            HashmapEntry* next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(map);
}