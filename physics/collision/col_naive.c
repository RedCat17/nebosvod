#include "../../core/world.h"
#include "../physics_config.h"
#include "../physics_context.h"
#include "collision.h"
#include <stdio.h>
#include <string.h>

// столкновения
void collisions_naive(PhysicsContext *ctx, PhysicsConfig *cfg) {
    memset(ctx->dead, 0, sizeof(char) * ctx->world.size);
    memset(ctx->world.move_x, 0, sizeof(float) * ctx->world.p_count);
    memset(ctx->world.move_y, 0, sizeof(float) * ctx->world.p_count);
    for (int i = 0; i < ctx->world.p_count; i++) {
        if (ctx->dead[i] == 1) continue;
        for (int j = i + 1; j < ctx->world.p_count; j++) {
            if (j <= i) continue;
            if (ctx->dead[j] == 1) continue;
            handle_collision(&ctx->world, cfg, i, j);
        }
    }

    for (int i = 0; i < ctx->world.p_count; i++) {
        if (ctx->dead[i] == 1) {
            // printf("deleting %d", i);
            delete_particle(&ctx->world, i);
        }
    }

    // // apply move
    // for (int i = 0; i < world->p_count; i++) {
    //     world->particles[i].x += world->move_x[i];
    //     world->particles[i].y += world->move_y[i];
    // }
}