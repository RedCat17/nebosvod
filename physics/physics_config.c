#include "physics_config.h"

#define EPS 0.01f // to prevent div by 0 in gravity
#define DT 0.01f // time step
#define SEPARATE 1.0f // how much to separate overlap in each step
// #define SEPARATE 0.1f // how much to separate overlap in each step
#define STEPS_PER_FRAME 100 // число шагов за кадр

#define DENSITY 1.0f // плотность частиц
#define G 10.0f // гравитационная константа
#define ELASTICITY 0.7f // упругость при столкновениях
// #define ELASTICITY 0.0f // упругость при столкновениях

#define COL_ALPHA 0.5f // fraction of v_esc under which merge occurs
// #define COL_ALPHA 0.0f // fraction of v_esc under which merge occurs
#define COL_BETA 1.5f // fraction of v_esc under which bounce occurs

#define BH_THETA 0.6f

void set_default_config(PhysicsConfig *cfg) {
    cfg->dt = DT;
    cfg->eps = EPS;

    cfg->g = G;
    cfg->density = DENSITY;
    
    cfg->elasticity = ELASTICITY;
    cfg->col_alpha = COL_ALPHA;
    cfg->col_beta = COL_BETA;

    cfg->separate = SEPARATE;
    
    cfg->bh_theta = BH_THETA;
};