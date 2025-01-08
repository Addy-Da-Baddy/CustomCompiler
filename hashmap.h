#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

typedef struct HashMap HashMap;

HashMap* hashmap_create();
void hashmap_free(HashMap* map);
int hashmap_put(HashMap* map, const char* key, int value);
int hashmap_get(HashMap* map, const char* key);

#endif // HASHMAP_H