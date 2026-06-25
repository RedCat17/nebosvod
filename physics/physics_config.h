#pragma once

typedef struct PhysicsConfig {
    float g;
    float density;
    float eps;
    float bh_theta;
    float elasticity;
    float col_alpha, col_beta;
    float dt;
    float separate;
} PhysicsConfig;

void set_default_config(PhysicsConfig *cfg);