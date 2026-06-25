#pragma once

typedef struct Particle {
    float x, y;
    float vx, vy;
    float mass, radius;
    // float density;
} Particle;

float get_radius(float mass, float density);

void init_particle(Particle *particle, float x, float y, float vx, float vy, float mass, float density);
