#pragma once

#include <raylib.h>
#include "physics/physics_config.h"
#include "physics/physics_context.h"

typedef struct AppContext {
    PhysicsConfig cfg;
    PhysicsContext ctx;
    Camera2D camera;
} AppContext;

void init_app(AppContext *app_ctx);

void start_app(AppContext *app_ctx);