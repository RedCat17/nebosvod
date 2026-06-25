#include <math.h>
#include <raylib.h>

#include "particle.h"

float get_radius(float mass, float density) {
    return sqrtf(mass / (density * PI));
}

void init_particle(Particle *particle, float x, float y, float vx, float vy, float mass, float density) {
    particle->x = x; 
    particle->y = y; 

    particle->vx = vx;
    particle->vy = vy;

    particle->mass = mass; 
    particle->radius = get_radius(particle->mass, density); 
}