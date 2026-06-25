#pragma once

#include "../../core/particle.h"
#include "../../util/const.h"
#include <stddef.h>

typedef struct UniformGrid {
    float root_x; // where grid starts in world space
    float root_y;

    float cell_size;

    size_t capacity_width;
    size_t capacity_height;

    size_t width; // number of cells
    size_t height;

    size_t cell_capacity;

    int *cells; // dynamic array of cells, each cell is array of indices of Particle in World.
    // indexing: cells[(cell_index * cell_capacity) + slot]
    int *counts;
} UniformGrid;

void init_grid(UniformGrid *grid, float root_x, float root_y, float cell_size, size_t width, size_t height, size_t capacity_width, size_t capacity_height, size_t cell_capacity);

void free_grid(UniformGrid *grid);

void clear_grid(UniformGrid *grid);

void grid_insert(UniformGrid *grid, float x, float y, int id);

void grid_insert_multicell(UniformGrid *grid, float x, float y, float r, int id);

int grid_query_neighbors(UniformGrid *grid, float x, float y, int *out_ids, int max_out);