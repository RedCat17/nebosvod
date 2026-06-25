#pragma once

#include <pthread.h>

#include "../core/world.h"
#include "gravity/bh.h"
#include "spatial/grid.h"
#include "../util/const.h"
#include "pair_cache.h"

typedef struct PhysicsContext {
    World world;

    BH_Tree tree;
    UniformGrid grid;
    PairCache cache;

    float temperature;
    float max_mass;
    float total_mass;
    float kin_energy;
    float pot_energy;
    float ang_momentum;

    float com_x;
    float com_y;

    char *dead; // same size as world.particles, 1 = removed
} PhysicsContext;

void init_ctx(PhysicsContext *ctx, int p_count);

void free_ctx(PhysicsContext *ctx);