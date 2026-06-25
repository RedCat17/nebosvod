#pragma once

#include <stdint.h>

typedef struct PairCache {
    uint64_t *keys;
    char *used;
    int capacity;
} PairCache;

void init_pair_cache(PairCache *cache, int capacity);

void free_pair_cache(PairCache *cache);

void clear_pair_cache(PairCache *cache);

int pair_cache_contains(PairCache *cache, uint64_t key);

void pair_cache_add(PairCache *cache, uint64_t key);

uint64_t make_pair_key(int a, int b);