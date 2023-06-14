#ifndef HASHMAP_H_
#define HASHMAP_H_

///
///@author Amelia Johnson (amelia-online)
///

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

unsigned long strhash(void*);
unsigned long identity(void*);
bool streq(void*, void*);
bool inteq(void*, void*);

typedef struct {
    void *key;
    void *value;
    bool deleted;
} Pair;

Pair Pair_default();


typedef struct HashMap {
    Pair* table;
    size_t size;
    unsigned long (*hash)(void*);
    bool (*equals)(void*, void*);
    void (*alloc)(Pair*, void*, void*);
    void (*dealloc)(Pair*);
} HashMap;

HashMap HashMap_make(size_t, unsigned long (*hash)(void*), bool (*equals)(void*, void*), 
    void (*alloc)(Pair*, void*, void*), void (*dealloc)(Pair*));
void HashMap_free(HashMap*);
int HashMap_count(const HashMap*);
void HashMap_clear(HashMap*);
bool HashMap_insert(HashMap*, void*, void*);
void *HashMap_get(const HashMap*, void*);
void *HashMap_strget(const HashMap*, const char*);
bool HashMap_remove(HashMap*, void*);
void HashMap_print(const HashMap*);
void HashMap_printf(const HashMap*, void (*format)(Pair*));
Pair* HashMap_entries(const HashMap*);
void** HashMap_keys(const HashMap*);
bool HashMap_contains(const HashMap*, void*);
void HashMap_resize(HashMap*, size_t);
void HashMap_allocator(HashMap*, void (*alloc)(Pair*, void*, void*));

#endif // HASHMAP_H
