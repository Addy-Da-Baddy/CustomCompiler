#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

static unsigned int hash(const char* key) {
    unsigned int hash = 5381;
    int c;
    
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash % TABLE_SIZE;
}

HashMap* hashmap_create() {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    if (!map) return NULL;
    
    memset(map->table, 0, sizeof(map->table));
    map->size = 0;
    return map;
}

void hashmap_put(HashMap* map, const char* key, int value) {
    unsigned int index = hash(key);
    
    // Create new entry
    Entry* entry = (Entry*)malloc(sizeof(Entry));
    entry->key = strdup(key);
    entry->value = value;
    entry->next = NULL;
    
    // If slot is empty
    if (map->table[index] == NULL) {
        map->table[index] = entry;
    } else {
        // Handle collision by adding to front of list
        Entry* current = map->table[index];
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                // Update existing value
                current->value = value;
                free(entry->key);
                free(entry);
                return;
            }
            current = current->next;
        }
        entry->next = map->table[index];
        map->table[index] = entry;
    }
    map->size++;
}

int hashmap_get(HashMap* map, const char* key, int* value) {
    unsigned int index = hash(key);
    Entry* current = map->table[index];
    
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            *value = current->value;
            return 1; // Found
        }
        current = current->next;
    }
    
    return 0; // Not found
}

void hashmap_free(HashMap* map) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Entry* current = map->table[i];
        while (current != NULL) {
            Entry* next = current->next;
            free(current->key);
            free(current);
            current = next;
        }
    }
    free(map);
}