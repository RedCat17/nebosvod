#pragma once

#include "particle.h"

// структура мира
typedef struct World {
    unsigned int size; 
    unsigned int p_count; 
    Particle* particles;

    unsigned int step;

    float *move_x;
    float *move_y;

    float *acc_x;
    float *acc_y;

    float *acc_new_x;
    float *acc_new_y;

    float minX, maxX;
    float minY, maxY;
    float maxR;
} World;

void init_world(World *world, int p_count);
void free_world(World *world);
void print_world(World *world);
void delete_particle(World *world, int a);