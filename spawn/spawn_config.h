#pragma once

#include <raylib.h>

typedef struct SpawnConfig {
    Rectangle bounds;
    float min_r, max_r;

    float min_mass, max_mass;
    
    float max_speed;
} SpawnConfig;