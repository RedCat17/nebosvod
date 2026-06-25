

#include "../core/world.h"
#include "../util/const.h"
#include "physics_context.h"
#include "collision/collision.h"
#include <string.h>

#include "bounds.h"

void kill_outliers(PhysicsContext *ctx) {
    memset(ctx->dead, 0, sizeof(char) * ctx->world.size);   

    for (int i = 0; i < ctx->world.p_count; i++) {
        Particle *prt = &ctx->world.particles[i];
        if (prt->x < -WORLD_HALF_SIZE || prt->x > WORLD_HALF_SIZE || prt->y < -WORLD_HALF_SIZE || prt->y > WORLD_HALF_SIZE)
            ctx->dead[i] = 1;
    }

    for (int i = 0; i < ctx->world.p_count; i++) {
        if (ctx->dead[i] == 1) {
            delete_particle(&ctx->world, i);
        }
    }
}