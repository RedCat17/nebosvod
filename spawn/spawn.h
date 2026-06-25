#pragma once

typedef struct World World;
typedef struct SpawnConfig SpawnConfig;
typedef struct PhysicsConfig PhysicsConfig;

void spawn_solar_system(World *world, SpawnConfig *scfg, PhysicsConfig *pcfg, float central_mass);

void spawn_box_cluster(World *world, SpawnConfig *scfg, PhysicsConfig *pcfg);

void spawn_test(World *world);