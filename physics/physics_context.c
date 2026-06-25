#include <stdlib.h>

#include "physics_context.h"
#include "spatial/grid.h"
#include "pair_cache.h"
#include "../util/const.h"

void init_ctx(PhysicsContext *ctx, int p_count) {
    init_world(&ctx->world, p_count);

    ctx->tree.nodes = malloc(sizeof(Node) * BH_MAX_NODES);
    ctx->dead = calloc(p_count, sizeof(char));
    init_grid(&ctx->grid, 0, 0, 0, 0, 0, 0, 0, 0);
    init_pair_cache(&ctx->cache, PAIR_CACHE_SIZE);
};

void free_ctx(PhysicsContext *ctx) {
    free_world(&ctx->world);

    if (ctx->tree.nodes) free(ctx->tree.nodes);
    if (ctx->dead) free(ctx->dead);
    free_grid(&ctx->grid);
    free_pair_cache(&ctx->cache);
};