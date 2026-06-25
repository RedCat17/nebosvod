#pragma once

typedef struct World World;
typedef struct PhysicsConfig PhysicsConfig;
typedef struct PhysicsContext PhysicsContext;

void collisions_naive(PhysicsContext *ctx, PhysicsConfig *cfg);