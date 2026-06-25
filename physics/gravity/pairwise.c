#include <math.h>
#include <string.h>

#include "../../core/world.h"
#include "../physics_config.h"

void newton_pairwise(World *world, PhysicsConfig *cfg, float *acc_x, float *acc_y) {
    memset(acc_x, 0, sizeof(float) * world->p_count);
    memset(acc_y, 0, sizeof(float) * world->p_count);
    for (int i = 0; i < world->p_count; i++) {
        Particle *prt1 = &world->particles[i];
        for (int j = i + 1; j < world->p_count; j++) {
            Particle *prt2 = &world->particles[j];

            float dx = prt2->x - prt1->x;
            float dy = prt2->y - prt1->y;

            float dist_sq = dx * dx + dy * dy;
            float dist = sqrtf(dist_sq);
            
            float inv_dist = 1.0f / (dist + cfg->eps);
            float inv_dist_sq = 1.0f / (dist_sq + cfg->eps);

            float acc1 = cfg->g * prt2->mass * inv_dist_sq;
            float acc2 = cfg->g * prt1->mass * inv_dist_sq;

            acc_x[i] += acc1 * dx * inv_dist;
            acc_y[i] += acc1 * dy * inv_dist;

            acc_x[j] -= acc2 * dx * inv_dist;
            acc_y[j] -= acc2 * dy * inv_dist;
        }
    }
}