#pragma once

typedef struct World World;
typedef struct PhysicsConfig PhysicsConfig;

void handle_collision(World *world, PhysicsConfig *cfg, int i, int j);