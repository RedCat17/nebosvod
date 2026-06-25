
#include <math.h>

#include "../core/world.h"

#include "gravity/bh.h"

#include "collision/collision.h"
#include "collision/col_naive.h"
#include "collision/col_grid.h"
#include "collision/col_grid_cells.h"

#include "physics_config.h"
#include "physics_context.h"
#include "bounds.h"

#include "integrator.h"

// действие гравитации
void update_acceleration(PhysicsContext *ctx, PhysicsConfig *cfg, float *acc_x, float *acc_y) {
    // newton_pairwise(world, acc_x, acc_y);
    // barnes_hut(world, acc_x, acc_y);
    barnes_hut_parallel(ctx, cfg, acc_x, acc_y);
}

// движение
void update_position(World *world, PhysicsConfig *cfg) {
    for (int i = 0; i < world->p_count; i++) {
        Particle *prt1 = &world->particles[i];
        prt1->x += prt1->vx * cfg->dt + 0.5f * world->acc_x[i] * cfg->dt * cfg->dt;
        prt1->y += prt1->vy * cfg->dt + 0.5f * world->acc_y[i] * cfg->dt * cfg->dt;
    }
}

// движение
void update_velocity(World *world, PhysicsConfig *cfg) {
    for (int i = 0; i < world->p_count; i++) {
        Particle *prt1 = &world->particles[i];
        prt1->vx += 0.5f * (world->acc_x[i] + world->acc_new_x[i]) * cfg->dt;
        prt1->vy += 0.5f * (world->acc_y[i] + world->acc_new_y[i]) * cfg->dt;
    }
}

void find_bounds(World *world) {
    world->minX = +INFINITY, world->maxX = -INFINITY;
    world->minY = +INFINITY, world->maxY = -INFINITY;
    world->maxR = 0;

    for (int i = 0; i < world->p_count; i++) {
        Particle *prt = &world->particles[i];
        world->minX = fminf(world->minX, prt->x - prt->radius);   
        world->maxX = fmaxf(world->maxX, prt->x + prt->radius);    
        world->minY = fminf(world->minY, prt->y - prt->radius);   
        world->maxY = fmaxf(world->maxY, prt->y + prt->radius);    
        world->maxR = fmaxf(world->maxR, prt->radius);    
    }
}

void step_world(PhysicsContext *ctx, PhysicsConfig *cfg) {
    ctx->world.step++;

    // find world boundaries
    find_bounds(&ctx->world);
    // old acc
    update_acceleration(ctx, cfg, ctx->world.acc_x, ctx->world.acc_y);
    // movement
    update_position(&ctx->world, cfg);
    // new acc
    update_acceleration(ctx, cfg, ctx->world.acc_new_x, ctx->world.acc_new_y);
    // update velocity
    update_velocity(&ctx->world, cfg);

    // swap buffers
    float *tmp;

    tmp = ctx->world.acc_x;
    ctx->world.acc_x = ctx->world.acc_new_x;
    ctx->world.acc_new_x = tmp;

    tmp = ctx->world.acc_y;
    ctx->world.acc_y = ctx->world.acc_new_y;
    ctx->world.acc_new_y = tmp;

    // find world boundaries
    find_bounds(&ctx->world);
    // collisions
    // collisions_naive(ctx, cfg);  
    // collisions_grid(ctx, cfg);    
    collisions_grid_by_cell(ctx, cfg);    
    
    // kill runaway particles
    kill_outliers(ctx);     
}