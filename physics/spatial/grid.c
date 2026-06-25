#include <assert.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "grid.h"

void init_grid(UniformGrid *grid, float root_x, float root_y, float cell_size, size_t width, size_t height, size_t capacity_width, size_t capacity_height, size_t cell_capacity) {
    grid->root_x = root_x;
    grid->root_y = root_y;
    grid->cell_size = cell_size;
    grid->width = width;
    grid->height = height;
    grid->capacity_width = capacity_width;
    grid->capacity_height = capacity_height;
    grid->cell_capacity = cell_capacity;

    grid->cells = calloc( capacity_width * capacity_height * cell_capacity, sizeof(int));
    grid->counts = calloc( capacity_width * capacity_height, sizeof(int));
}

void free_grid(UniformGrid *grid) {
    free(grid->cells);
    free(grid->counts);
}

void clear_grid(UniformGrid *grid) {
    memset(grid->counts, 0, sizeof(int) * grid->height * grid->width);
}

void get_cell_by_pos(UniformGrid *grid, float x, float y, int *i, int *j) {
    *i = (y - grid->root_y) / grid->cell_size;
    *j = (x - grid->root_x) / grid->cell_size;
}

void grid_insert(UniformGrid *grid, float x, float y, int id) {
    int i, j;
    get_cell_by_pos(grid, x, y, &i, &j);
    if (!((i >= 0 && i < grid->height) && (j >= 0 && j < grid->width))) {
        printf("(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", x, y, grid->root_x, grid->root_y, 
            grid->root_x + grid->width * grid->cell_size, 
            grid->root_y + grid->height * grid->cell_size);
        // printf("(%.2f, %.2f), (%.2f, %.2f)", i, j, grid->width, grid->height);
        printf("p (%d, %d), g (%d, %d)\n", i, j, grid->height, grid->width);
    }
    assert((i >= 0 && i < grid->height) && (j >= 0 && j < grid->width));

    size_t index = i * grid->width + j;
    size_t offset = (size_t)index * (size_t)grid->cell_capacity + (size_t)grid->counts[index];
    int cell_count = grid->width * grid->height;
    int capacity_count = grid->capacity_width * grid->capacity_height;
    assert(index >= 0 && index < cell_count);
    assert(grid->cells != NULL);
    // printf("%d %d %d\n", index, cell_count, capacity_count);

    assert(grid->counts[index] < grid->cell_capacity);
    grid->cells[offset] = id;
    grid->counts[index]++;
}

void grid_insert_multicell(UniformGrid *grid, float x, float y, float r, int id) {
    int left = floor((x - r - grid->root_x) / grid->cell_size);
    int right = floor((x + r - grid->root_x) / grid->cell_size);

    int top = floor((y - r - grid->root_y) / grid->cell_size);
    int bottom = floor((y + r - grid->root_y) / grid->cell_size);

    // if (!((i >= 0 && i < grid->height) && (j >= 0 && j < grid->width))) {
    //     printf("(%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n", x, y, grid->root_x, grid->root_y, 
    //         grid->root_x + grid->width * grid->cell_size, 
    //         grid->root_y + grid->height * grid->cell_size);
    //     // printf("(%.2f, %.2f), (%.2f, %.2f)", i, j, grid->width, grid->height);
    //     printf("p (%d, %d), g (%d, %d)\n", i, j, grid->height, grid->width);
    // }
    // assert((i >= 0 && i < grid->height) && (j >= 0 && j < grid->width));

    for (int m = top; m <= bottom; m++) {
        int row_base = m * grid->width;
        for (int n = left; n <= right; n++) {
            int index = row_base + n;
            assert(grid->counts[index] < grid->cell_capacity);
            grid->cells[index * grid->cell_capacity + grid->counts[index]] = id;
            grid->counts[index]++;
        }
    }        
}

int grid_query_neighbors(UniformGrid *grid, float x, float y, int *out_ids, int max_out) {
    int i, j;
    get_cell_by_pos(grid, x, y, &i, &j);
    assert((i >= 0 && i < grid->height) && (j >= 0 && j < grid->width));

    int count = 0;

    // clamp query to grid borders
    int top = i - 1 >= 0? i - 1 : 0;
    int bottom = i + 1 <= grid->height - 1? i + 1 : grid->height - 1;
    int left = j - 1 >= 0? j - 1 : 0;
    int right = j + 1 <= grid->width - 1? j + 1 : grid->width - 1;

    for (int m = top; m <= bottom; m++) {
        int row_base = m * grid->width;
        for (int n = left; n <= right; n++) {
            int index = row_base + n;
            for (int k = 0; k < grid->counts[index]; k++) {
                if (count < max_out) {
                    out_ids[count] = grid->cells[index * grid->cell_capacity + k];
                    count++;
                }
            }
        }
    }        
    if (count == max_out) {
        printf("QUERY SATURATED\n");
    }

    return count;
}