#include <stdio.h>
#include <stdlib.h>

#include "world.h"

void init_world(World *world, int size) {
    world->size = size;
    world->p_count = size;
    world->particles = (Particle*) calloc(size, sizeof(Particle));
    world->move_x = calloc(size, sizeof(float));
    world->move_y = calloc(size, sizeof(float));
    world->acc_x = calloc(size, sizeof(float));
    world->acc_y = calloc(size, sizeof(float));
    world->acc_new_x = calloc(size, sizeof(float));
    world->acc_new_y = calloc(size, sizeof(float));
    world->step = 0;
}

void free_world(World *world) {
    if (world->particles) free(world->particles);
    if (world->move_x) free(world->move_x);
    if (world->move_y) free(world->move_y);
    if (world->acc_x) free(world->acc_x);
    if (world->acc_y) free(world->acc_y);
    if (world->acc_new_x) free(world->acc_new_x);
    if (world->acc_new_y) free(world->acc_new_y);
}

// вывод информации о мире в консоль
void print_world(World *world) {
    printf("World at step %d:\n", world->step);
    for (int i = 0; i < world->p_count; i++) {
        Particle *prtcl = &world->particles[i];
        // printf("X: %.1f | Y: %.1f | VX: %.1f | VY: %.1f | mass: %.1f | radius: %.1f\n",
        printf("X: %.10f | Y: %.10f | VX: %.10f | VY: %.10f | mass: %.1f | radius: %.1f\n",
           prtcl->x, prtcl->y, prtcl->vx, prtcl->vy, prtcl->mass, prtcl->radius);
    }
}

void delete_particle(World *world, int a) {
    // printf("deleting particle %d\n", a);
    // print_world(world);
    world->particles[a] = world->particles[world->p_count - 1]; // заменя удаленной на последнюю
    world->p_count--;
    // print_world(world);
}