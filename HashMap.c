#include "HashMap.h"

/**
 * @author Amelia Johnson (amelia-online)
*/

int clamp(const HashMap* map, unsigned long hash) {
    return hash % map->size;
}

/// @brief A hash function for strings. Use this as the hash
/// function if your keys are of type const char*. This is based on the djb2
/// hash function.
/// @param key A pointer to the key.
/// @return A (hopefully) unique integer ID.
unsigned long strhash(void *key) {
    const char* str = (const char*)key;

    unsigned long sum = 0;
    int current;
    int multiplier = 1;
    while ((current = *str++)) {
        sum += (current << 3) << multiplier++;
    }
    return sum;
}

/// @brief Checks if two strings are equal; uses strcmp.
/// Most of the work in here is NULL-checking and casting.
/// @param lhs key 1
/// @param rhs key 2
/// @return True if the strings are equal, false otherwise.
bool streq(void* lhs, void* rhs) {
    if (!rhs || !lhs)
        return false;

    const char* key1 = (const char*)lhs;
    const char* key2 = (const char*)rhs;

    return strcmp(key1, key2) == 0;
}

/// @brief Checks if two integers are equal. This function is
/// mostly for NULL-checking and type casting, and is
/// required in order for the map to work if your key is int.
/// @param key1 <<
/// @param key2 <<
/// @return True if the keys' values are equal, false otherwise.
bool inteq(void* key1, void* key2) {
    if (!key1 || !key2)
        return false;

    int k1 = *((int*)key1);
    int k2 = *((int*)key2);
    return k1 == k2;
}

/// @brief Returns a Pair* of the entries of the map.
/// @param map The map to read.
/// @return See above.
Pair* HashMap_entries(const HashMap* map) {

    if (HashMap_count(map) == 0)
        return NULL;

    Pair* entries = (Pair*)malloc(sizeof(Pair)*HashMap_count(map));
    int pos = 0;
    for (int i = 0; i < map->size; i++)
        if (!map->table[i].deleted)
            entries[pos++] = map->table[i];
    return entries;
}

/// @brief Returns a void** to the keys of the map.
/// @param map The map to read.
/// @return See above.
void** HashMap_keys(const HashMap* map) {

     if (HashMap_count(map) == 0)
        return NULL;

    void** keys = (void**)malloc(sizeof(void*)*HashMap_count(map));
    int pos = 0;
    for (int i = 0; i < map->size; i++)
        if (!map->table[i].deleted)
            keys[pos++] = map->table[i].key;
    return keys;
}

/// @brief A hash function for type int.
/// Because all integers are unique, this function simply returns
/// the integer pointed to by the given key. Hence, this is the
/// identity function; use this as the hash function.
/// @param key A pointer to the key.
/// @return The integer pointed to by key as an unsigned long.
unsigned long identity(void* key) {
    int* val = (int*)key;
    return (unsigned long)*val;
}

/// @brief Creates a default initialized Pair.
/// @return See above.
Pair Pair_default() {
    Pair p = {
        .deleted = true,
        .key = NULL,
        .value = NULL,
    };
    return p;
}

/// @brief Creates a HashMap of size 'size,' can map any type to another.
/// @param size The size of the backing-table.
/// @param hash The hash function to use. Use strhash if your keys are Strings.
/// @param equals The function that tells when two keys are equal.
/// @param allocator The function that allocates memory for the values.
/// @param deallocator The function that handles the freeing of memory for the values.
/// @return An empty HashMap.
HashMap HashMap_make(size_t size, unsigned long (*hash)(void*), bool (*equals)(void*, void*),
    void (*allocator)(Pair*, void*, void*), void (*deallocator)(Pair*)) {
    Pair* table = (Pair*)malloc(sizeof(Pair)*size);

    for (int i = 0; i < size; i++)
        table[i] = Pair_default();

    HashMap map = {
        .size = size,
        .table = table,
        .hash = hash,
        .equals = equals,
        .alloc = allocator,
        .dealloc = deallocator,
    };

    return map;
}

/// @brief Frees the resources of the given HashMap.
/// @param map The map to free.
void HashMap_free(HashMap* map) {
    free(map->table);
}

/// @brief Removes all elements of the map.
/// @param map The map to modify.
void HashMap_clear(HashMap* map) {
    HashMap_free(map);
    map->table = (Pair*)malloc(sizeof(Pair)*map->size);
    for (int i = 0; i < map->size; i++)
        map->table[i] = Pair_default();
}

/// @brief Returns the number of entries in the map.
/// @param map The map to read.
/// @return The number of entries in the map.
int HashMap_count(const HashMap* map) {
    int sum = 0;
    for (int i = 0; i < map->size; i++) {
        if (map->table[i].key)
            sum++;
    }
    return sum;
}

/// @brief Uses linear probing to insert an element; if the key already exists, its value will be replaced.
/// @param map The map to modify.
/// @param key The key of the element.
/// @param value The value of the element.
/// @return true if element successfully inserted, false otherwise.
bool HashMap_insert(HashMap* map, void* key, void* value) {

    int index = clamp(map, map->hash(key));

    int numTraversed = 0;
    int offset = index;
    while (numTraversed < map->size) {
        if (map->equals(map->table[offset].key, key)) {
            map->dealloc(&map->table[offset]);
            map->alloc(&map->table[offset], key, value);
            return true;
        } else if (map->table[offset].deleted) {
            map->alloc(&map->table[offset], key, value);
            map->table[offset].deleted = false;
            return true;
        }   

        numTraversed++;
        if (offset >= map->size - 1)
            offset = 0;
        else
            offset++;
    }
    return false;
}

/// @brief If your key is of type `const char*`, this function
/// acts as a shortut by simply entering in the key directly.
/// @param map The map to read.
/// @param key The key to search for.
/// @return A void* to the value mapped to the key.
void *HashMap_strget(const HashMap* map, const char* key) {
    return HashMap_get(map, key);
}

/// @brief Returns the item mapped to the key.
/// @param map The HashMap to read.
/// @param key The key mapped to the item.
/// @return a void* to the item; NULL otherwise.
void *HashMap_get(const HashMap* map, void *key) {

    int index = clamp(map, map->hash(key));

    int numTraversed = 0;
    int offset = index;
    while (numTraversed < map->size) {
       
        if (map->equals(map->table[offset].key, key)) {
            return map->table[offset].value;
        }

        numTraversed++;
        if (offset >= map->size - 1)
            offset = 0;
        else
            offset++;
    }

    return NULL;
}

/// @brief Removes the element with the given key.
/// @param map The map to modify.
/// @param key The key to remove.
/// @return True if the element was removed; false otherwise.
bool HashMap_remove(HashMap* map, void* key) {

    int index = clamp(map, map->hash(key));

    int numTraversed = 0;
    int offset = index;
    while (numTraversed < map->size) {
       
        if (map->equals(map->table[offset].key, key)) {
            map->dealloc(&map->table[offset]);
            map->table[offset] = Pair_default();
            return true;
        }

        numTraversed++;
        if (offset >= map->size - 1)
            offset = 0;
        else
            offset++;
    }

    return false;
}

/// @brief Prints a representation of the HashMap.
/// @param map The map to print.
void HashMap_print(const HashMap* map) {
    for (int i = 0; i < map->size; i++)
        printf("[k: %p - v: %p]", map->table[i].key, map->table[i].value);
    printf("\n");
}


/// @brief Allows for printing the contents of the map, given you handle how to print
/// the non-null values.
/// @param map The map to read.
/// @param format A function pointer that points to a function that prints non-null values.
void HashMap_printf(const HashMap* map, void (*format)(Pair*)) {
    for (int i = 0; i < map->size; i++) {
        if (!map->table[i].key) {
            printf("[None]");
            continue;
        }
        format(&map->table[i]);
    }
    printf("\n");
}

/// @brief Checks if the key is present within the given map.
/// @param map The map to read.
/// @param key The key to search for.
/// @return True if the key is present, false otherwise.
bool HashMap_contains(const HashMap* map, void* key) {
    int index = clamp(map, map->hash(key));

    int traversed = 0;
    while (traversed < map->size) {
        if (map->equals(map->table[index].key, key))
            return true;

        if (index >= map->size - 1)
            index = 0;
        else
            ++index;
        ++traversed;
    }
    return false;
}

/// @brief Resizes the HashMap to the provided size.
/// @param map The map to resize.
/// @param size The desired size of the map.
void HashMap_resize(HashMap* map, size_t size) {
    map->table = (Pair*)realloc(map->table, size);
    map->size = size;
}
