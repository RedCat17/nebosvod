#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../../core/world.h"
#include "../physics_config.h"
#include "../physics_context.h"
#include "collision.h"
#include "../spatial/grid.h"
#include "../pair_cache.h"
#include "../../util/const.h"

#include "col_grid_cells.h"
// #define COL_MAX_CANDIDATES 1024
#define COL_MAX_CANDIDATES 8192
#define CELL_SIZE 100.0f

static void build_grid_multicell(PhysicsContext *ctx) {
    float cell_size = CELL_SIZE;
    float root_x = ctx->world.minX - cell_size;
    float root_y = ctx->world.minY - cell_size;
    // printf("w (%.2f %.2f), (%.2f %.2f)\n", ctx->world.minX, ctx->world.minY, ctx->world.maxX, ctx->world.maxY);
    int width = (int)ceilf((ctx->world.maxX - root_x) / cell_size) + 1;
    int height = (int)ceilf((ctx->world.maxY - root_y) / cell_size) + 1;

    // printf("g %d %d, %d %d, %d %d\n", width, height, ctx->grid.width, ctx->grid.height, ctx->grid.capacity_width, ctx->grid.capacity_height);
    if (ctx->grid.capacity_width == 0) {
        // printf("new grid\n");
        free_grid(&ctx->grid);
        init_grid(&ctx->grid, root_x, root_y, cell_size, width, height, width * 2, height * 2, GRID_PAIRS_CELL_CAPACITY);
    }
    else {
        if (width > ctx->grid.capacity_width || height > ctx->grid.capacity_height) {
        // printf("resizing grid\n");
        int new_capacity_width = ctx->grid.capacity_width * 2;
        int new_capacity_height = ctx->grid.capacity_height * 2;
        free_grid(&ctx->grid);
        init_grid(&ctx->grid, root_x, root_y, cell_size, width, height, new_capacity_width, new_capacity_height, GRID_PAIRS_CELL_CAPACITY);
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
        grid_insert_multicell(&ctx->grid, prt->x, prt->y, prt->radius, i);
    } 
}

// столкновения
void collisions_grid_by_cell(PhysicsContext *ctx, PhysicsConfig *cfg) {
    build_grid_multicell(ctx); 
    memset(ctx->dead, 0, sizeof(char) * ctx->world.size);
    memset(ctx->world.move_x, 0, sizeof(float) * ctx->world.p_count);
    memset(ctx->world.move_y, 0, sizeof(float) * ctx->world.p_count);

    clear_pair_cache(&ctx->cache);
    for (int i = 0; i < ctx->grid.height; i++) {
        int row_base = i * ctx->grid.width;
        for (int j = 0; j < ctx->grid.width; j++) {
            int index = row_base + j;
            for (int m = 0; m < ctx->grid.counts[index]; m++) {

                int id_a = ctx->grid.cells[index * GRID_PAIRS_CELL_CAPACITY + m];

                if (ctx->dead[id_a] == 1) continue;
                
                for (int n = m + 1; n < ctx->grid.counts[index]; n++) {

                    int id_b = ctx->grid.cells[index * GRID_PAIRS_CELL_CAPACITY + n];

                    if (ctx->dead[id_b] == 1) continue;

                    // printf("checking a=%d, b=%d\n", id_a, id_b);

                    uint64_t key = make_pair_key(id_a, id_b);

                    if (!pair_cache_contains(&ctx->cache, key)) {
                        pair_cache_add(&ctx->cache, key);
                        // printf("handling a=%d, b=%d\n", id_a, id_b);
                        handle_collision(&ctx->world, cfg, id_a, id_b);
                        // printf("handled.\n");
                    }
                }
            }
        }
    }

    // apply move
    for (int i = 0; i < ctx->world.p_count; i++) {
        ctx->world.particles[i].x += ctx->world.move_x[i];
        ctx->world.particles[i].y += ctx->world.move_y[i];
    }

    // for (int i = 0; i < ctx->world.p_count; ) {
    //     if (ctx->dead[i] == 1) {
    //         printf("deleting %d", i);
    //         delete_particle(&ctx->world, i);
    //     } else {
    //         i++;
    //     }
    // }

    for (int i = 0; i < ctx->world.p_count; i++) {
        if (ctx->dead[i] == 1) {
            // printf("deleting %d\n", i);
            delete_particle(&ctx->world, i);
        }
    }
}