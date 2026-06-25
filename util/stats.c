#include "../physics/physics_context.h"
#include "../physics/physics_config.h"
#include <math.h>

#include "stats.h"

void update_stats(PhysicsContext *ctx, PhysicsConfig *cfg) {
    ctx->max_mass = 0;
    ctx->total_mass = 0;
    ctx->temperature = 0;
    ctx->kin_energy = 0;
    ctx->pot_energy = 0;
    ctx->ang_momentum = 0;
    for (int i = 0; i < ctx->world.p_count; i++) {
        Particle *prt1 = &ctx->world.particles[i];
        ctx->total_mass += prt1->mass;
        // if (prt1->mass > world->max_mass) world->max_mass = prt1->mass;
        if (prt1->mass > ctx->max_mass && i > 0) ctx->max_mass = prt1->mass;
        ctx->kin_energy += (prt1->vx * prt1->vx + prt1->vy * prt1->vy) * prt1->mass;
        ctx->ang_momentum += (prt1->vy - prt1->vx) * prt1->mass;
        ctx->com_x += prt1->mass * prt1->x;
        ctx->com_y += prt1->mass * prt1->y;
        for (int j = i + 1; j < ctx->world.p_count; j++) {
            Particle *prt2 = &ctx->world.particles[j];

            float dx = prt2->x - prt1->x;
            float dy = prt2->y - prt1->y;

            float dist = sqrtf(dx * dx + dy * dy + cfg->eps*cfg->eps);

            ctx->pot_energy -= cfg->g * prt1->mass * prt2->mass / dist;
        }
    }
    ctx->com_x /= ctx->total_mass;
    ctx->com_y /= ctx->total_mass;
    ctx->kin_energy *= 0.5f;
    ctx->temperature = ctx->kin_energy / ctx->world.p_count;
}