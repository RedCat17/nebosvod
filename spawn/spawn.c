
#include <math.h>
#include <stdlib.h>

#include "../core/world.h"
#include "../spawn/spawn_config.h"
#include "../physics/physics_config.h"

#include "../spawn/spawn.h"

void init_rand_p(Particle *particle, SpawnConfig *cfg, float density) {
    float x = cfg->bounds.x + (float)rand() / (float)RAND_MAX * cfg->bounds.width; 
    float y = cfg->bounds.y + (float)rand() / (float)RAND_MAX * cfg->bounds.height; 

    float mass = cfg->min_mass + (float)rand() / (float)RAND_MAX * (cfg->max_mass - cfg->min_mass); 

    init_particle(particle, x, y, 0, 0, mass, density);
}

void init_rand_p_radial(Particle *particle, SpawnConfig *cfg, float density) {
    float angle = (float)rand() / (float)RAND_MAX * 2 * PI;
    float r = (float)rand() / (float)RAND_MAX * (cfg->max_r - cfg->min_r) + cfg->min_r;

    float x = cos(angle) * r;
    float y = sin(angle) * r;

    float mass = cfg->min_mass + (float)rand() / (float)RAND_MAX * (cfg->max_mass - cfg->min_mass); 

    init_particle(particle, x, y, 0, 0, mass, density);
}

void set_random_speed(Particle *particle, SpawnConfig *cfg) {
    // случайное направление, случайная скорость
    float angle = (float)rand() / (float)RAND_MAX * 2 * PI;
    float speed = (float)rand() / (float)RAND_MAX * cfg->max_speed;

    float vx = cos(angle) * speed;
    float vy = sin(angle) * speed;

    particle->vx = vx;
    particle->vy = vy;
}

void put_in_orbit(Particle *p, float spawn_central_mass, float g) {
    // первая космическая скорость, по часовой стрелке
    float dist = sqrtf(p->x * p->x + p->y * p->y);
    float vx = p->y / dist * sqrtf(g * spawn_central_mass / dist);
    float vy = - p->x / dist * sqrtf(g * spawn_central_mass / dist);

    p->vx = vx;
    p->vy = vy;
}

void spawn_solar_system(World *world, SpawnConfig *scfg, PhysicsConfig *pcfg, float central_mass) {
for (int i = 0; i < world->p_count; i++) {
        Particle *p = &world->particles[i];
        init_rand_p_radial(p, scfg, pcfg->density);
        put_in_orbit(p, central_mass, pcfg->g);
    }
    init_particle(&world->particles[0], 0, 0, 0, 0, central_mass, pcfg->density);

}

void spawn_box_cluster(World *world, SpawnConfig *scfg, PhysicsConfig *pcfg) {
for (int i = 0; i < world->p_count; i++) {
        Particle *p = &world->particles[i];
        init_rand_p(p, scfg, pcfg->density);
        set_random_speed(p, scfg);
    }

}

void spawn_test(World *world) {
        // init_particle(&world->particles[0], -100, 0, 1, 0.1, 1000);
    // init_particle(&world->particles[1], 100, 0, -1, 0.2, 1000);
    // init_particle(&world->particles[0], -100, 0, 10, 0, 50, 1);
    // init_particle(&world->particles[1], 100, 0, -10, 0, 100, 1);
    // init_particle(&world->particles[0], -100, 0, 0, 0, 1000, 1); // gravity bounce energy conservation test
    // init_particle(&world->particles[1], 100, 0, -0, 0, 1000, 1);
    // init_particle(&world->particles[0], -50, 0, 0, 6, 1000); // orbit test
    // init_particle(&world->particles[1], 50, 0, -0, -6, 1000);
    // init_particle(&world->particles[0], 0, 0, 0, 1.8, 1000); // elliptical orbit test
    // init_particle(&world->particles[1], 50, 0, -0, -18, 100);
    // init_particle(&world->particles[0], -50, 0, 0, 0, 1000); // gravity bounce energy conservation test
    // init_particle(&world->particles[1], 50, 0, -0, 0, 1000);
    // init_particle(&world->particles[0], -200, 10, 0, 0, 10000); // gravity bounce energy conservation test
    // init_particle(&world->particles[1], 200, 10, -0, 0, 10000);

    // init_particle(&world->particles[0], 90, -50, -1, 1.2, 10); // BH tree placement test
    // init_particle(&world->particles[1], 110, -70, 0, 0, 10); 
    // init_particle(&world->particles[2], -20, 60, 0, 0, 10);
    // init_particle(&world->particles[3], -60, 60, 0, 0, 10);
    // init_particle(&world->particles[4], -100, 100, 0, 0, 10);
    // init_particle(&world->particles[5], 60, 100, 0, 0, 10); 
    // // init_particle(&world->particles[5], 60, 100, 0, 0, 0.1); 
    // // init_particle(&world->particles[6], 60, 101, 0, 0, 0.1); 

    // init_particle(&world->particles[0], -100, -10, 0, 0, 1000, 1); // stability test
    // init_particle(&world->particles[1], 100, 10, -0, 0, 1000, 1);

    init_particle(&world->particles[0], -100, 0, 0, 0, 1000, 1); // triple stability test
    init_particle(&world->particles[1], 100, 0, -0, 0, 1000, 1);
    init_particle(&world->particles[2], -0, 150, -0, 0, 1000, 1);

    // init_particle(&world->particles[0], -100, 0, 0, 0, 1000, 1); // quad stability test
    // init_particle(&world->particles[1], 100, 0, -0, 0, 1000, 1);
    // init_particle(&world->particles[2], -200, 0, -0, 0, 1000, 1);
    // init_particle(&world->particles[3], 200, 0, -0, 0, 1000, 1);
}