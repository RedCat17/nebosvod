#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pair_cache.h"

void init_pair_cache(PairCache *cache, int capacity) {
    cache->keys = malloc(sizeof(uint64_t) * capacity);
    cache->used = calloc(capacity, sizeof(char));
    cache->capacity = capacity;
}

void free_pair_cache(PairCache *cache) {
    if (cache->keys) {
        free(cache->keys);
        free(cache->used);
    }
}

void clear_pair_cache(PairCache *cache) {
    memset(cache->used, 0, cache->capacity);
}

uint64_t make_pair_key(int a, int b) {
    if (a > b) {
        int temp = a;
        a = b;
        b = temp;
    }

    return ((uint64_t)a << 32 | (uint32_t)b);
}

uint32_t hash_u64(uint64_t x) {
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return (uint32_t)x;
}

int pair_cache_contains(PairCache *cache, uint64_t key) {
    // printf("contains %lu?\n", key);
    uint32_t mask = cache->capacity - 1;
    uint32_t index = hash_u64(key) & mask;

    while (cache->used[index]) {
        if (cache->keys[index] == key) {
            // printf("yes\n");
            return 1;
        }
        // printf("occupied: %u\n", index);
        index = (index + 1) & mask;
    }

    // printf("no\n");
    return 0;
}

void pair_cache_add(PairCache *cache, uint64_t key) {
    // printf("adding %lu\n", key);

    uint32_t mask = cache->capacity - 1;
    uint32_t index = hash_u64(key) & mask;

    while (cache->used[index]) {
        if (cache->keys[index] == key) 
            return;
        index = (index + 1) & mask;
        // printf("occupied\n");
    }

    cache->used[index] = 1;
    cache->keys[index] = key;

    // printf("added %lu\n", key);
}