#pragma once

typedef struct PhysicsConfig PhysicsConfig;
typedef struct PhysicsContext PhysicsContext;

// check collisions using spatial grid to only check neighbors
// works bad with particles of very different size
void collisions_grid(PhysicsContext *ctx, PhysicsConfig *cfg);