#include "../../core/world.h"
#include "../physics_config.h"
#include "../physics_context.h"
#include "collision.h"
#include "../spatial/grid.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "col_grid.h"

#define COL_MAX_CANDIDATES 1024
// #define COL_MAX_CANDIDATES 8192

void build_grid(PhysicsContext *ctx) {
    float cell_size = ctx->world.maxR * 2 * 1.1f;
    float root_x = ctx->world.minX - cell_size;
    float root_y = ctx->world.minY - cell_size;
    // printf("w (%.2f %.2f), (%.2f %.2f)\n", ctx->world.minX, ctx->world.minY, ctx->world.maxX, ctx->world.maxY);
    int width = (int)ceilf((ctx->world.maxX - root_x) / cell_size) + 1;
    int height = (int)ceilf((ctx->world.maxY - root_y) / cell_size) + 1;

    // printf("g %d %d, %d %d, %d %d\n", width, height, ctx->grid.width, ctx->grid.height, ctx->grid.capacity_width, ctx->grid.capacity_height);
    if (ctx->grid.capacity_width == 0) {
        // printf("new grid\n");
        free_grid(&ctx->grid);
        init_grid(&ctx->grid, root_x, root_y, cell_size, width, height, width * 2, height * 2, GRID_CELL_CAPACITY);
    }
    else {
        if (width > ctx->grid.capacity_width || height > ctx->grid.capacity_height) {
        // printf("resizing grid\n");
        int new_capacity_width = ctx->grid.capacity_width * 2;
        int new_capacity_height = ctx->grid.capacity_height * 2;
        free_grid(&ctx->grid);
        init_grid(&ctx->grid, root_x, root_y, cell_size, width, height, new_capacity_width, new_capacity_height, GRID_CELL_CAPACITY);
        }
        else {
            // printf("clear grid\n");
            clear_grid(&ctx->grid);
            ctx->grid.cell_size = cell_size;
            ctx->grid.root_x = root_x;
            ctx->grid.root_y = root_y;
            ctx->grid.height = height;
            ctx->grid.width = width;
        }
    }

    for (int i = 0; i < ctx->world.p_count; i++) {
        Particle *prt = &ctx->world.particles[i];
        grid_insert(&ctx->grid, prt->x, prt->y, i);
    } 
}

// столкновения
void collisions_grid(PhysicsContext *ctx, PhysicsConfig *cfg) {
    build_grid(ctx); 
    memset(ctx->dead, 0, sizeof(char) * ctx->world.size);
    memset(ctx->world.move_x, 0, sizeof(float) * ctx->world.p_count);
    memset(ctx->world.move_y, 0, sizeof(float) * ctx->world.p_count);

    if (ctx->world.step % 2 == 0)
        for (int i = 0; i < ctx->world.p_count; i++) {
            int candidates[COL_MAX_CANDIDATES];
            if (ctx->dead[i] == 1) continue;
            Particle *prt = &ctx->world.particles[i];
            int c_count = grid_query_neighbors(&ctx->grid, prt->x, prt->y, candidates, COL_MAX_CANDIDATES);

            // printf("i=%d candidates=%d\n", i, c_count);
            for (int j = 0; j < c_count; j++) {
                int j_id = candidates[j];
                if (j_id <= i) continue;
                if (ctx->dead[j_id] == 1) continue;
                handle_collision(&ctx->world, cfg, i, j_id);
            }
        }
    else
        for (int i = ctx->world.p_count - 1; i > 0; i--) {
            int candidates[COL_MAX_CANDIDATES];
            if (ctx->dead[i] == 1) continue;
            Particle *prt = &ctx->world.particles[i];
            int c_count = grid_query_neighbors(&ctx->grid, prt->x, prt->y, candidates, COL_MAX_CANDIDATES);

            // printf("i=%d candidates=%d\n", i, c_count);
            for (int j = 0; j < c_count; j++) {
                int j_id = candidates[j];
                if (j_id >= i) continue;
                if (ctx->dead[j_id] == 1) continue;
                handle_collision(&ctx->world, cfg, i, j_id);
            }
        }

    // apply move
    for (int i = 0; i < ctx->world.p_count; i++) {
        ctx->world.particles[i].x += ctx->world.move_x[i];
        ctx->world.particles[i].y += ctx->world.move_y[i];
    }

    for (int i = 0; i < ctx->world.p_count; i++) {
        if (ctx->dead[i] == 1) {
            // printf("deleting %d\n", i);
            delete_particle(&ctx->world, i);
        }
    }
}