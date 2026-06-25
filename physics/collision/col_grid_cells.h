#pragma once

typedef struct PhysicsConfig PhysicsConfig;
typedef struct PhysicsContext PhysicsContext;

void collisions_grid_by_cell(PhysicsContext *ctx, PhysicsConfig *cfg);