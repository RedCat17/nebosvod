#pragma once

typedef struct PhysicsContext PhysicsContext;
typedef struct PhysicsConfig PhysicsConfig;

void step_world(PhysicsContext *ctx, PhysicsConfig *cfg);