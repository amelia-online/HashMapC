#include <stdio.h>
#include "HashMap.h"

/**
 * 
 * This is an example of how to use my HashMap implementation.
 * The user must provide an allocate, deallocate, hash, and key
 * equality function in order for the HashMap to work. In certain
 * cases, these functions are provided for you. For example, if
 * your key type is String or int, hash and equality functions
 * are provided for you; see `strhash`, `streq`, `identity`, and
 * `inteq`.
 * 
 * @author Amelia Johnson (amelia-online)
 * 
*/

void format(Pair* p) {
    printf("[%s -> %d]", (const char*)p->key, *((int*)p->value));
}

void allocate(Pair* p, void* key, void* value) {
    const char* k = (const char*)key;
    size_t size = strlen(k);
    char* res = (char*)malloc(size+1);
    strcpy(res, k);
    p->key = res;
    int* vres = (int*)malloc(sizeof(int));
    *vres = *((int*)value);
    p->value = vres;
}

void deallocate(Pair* p) {
    free((char*)p->key);
    free((int*)p->value);
}

int main() {
    HashMap map = HashMap_make(4, &strhash, &streq, &allocate, &deallocate);

    char* A = "A";
    char* B = "B";
    char* C = "C";
    int a = 1;
    int b = 2;
    int c = 3;

    HashMap_insert(&map, A, &a);
    HashMap_insert(&map, B, &b);
    HashMap_insert(&map, C, &c);

    c = 4;
    
    HashMap_printf(&map, &format);

    HashMap_free(&map);
    return 0;
}
