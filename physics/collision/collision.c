#include <math.h>
#include <stdio.h>

#include "../../core/world.h"
#include "../physics_config.h"
#include "../physics_context.h"

// отскок
void bounce(PhysicsConfig *cfg, int i, int j, float dx, float dy, float dist, float v_rel_x, float v_rel_y, float mass_sum, float r, Particle* prt1, Particle* prt2) {
    // printf("Bounce!\n");
    // normal
    float nx = dx / dist;
    float ny = dy / dist;
    
    // v_rel projection on normal
    float scalar = v_rel_x * nx + v_rel_y * ny;
    
    if (scalar < 0) { // if moving to each other
        // float v_t_x = v_rel_x - v_n_x;
        // float v_t_y = v_rel_y - v_n_y;

        // float v_n_x = nx * scalar;
        // float v_n_y = ny * scalar;

        // prt1->vx += v_n_x * 2 * prt2->mass / mass_sum * ELASTICITY;
        // prt1->vy += v_n_y * 2 * prt2->mass / mass_sum * ELASTICITY;

        // prt2->vx -= v_n_x * 2 * prt1->mass / mass_sum * ELASTICITY;
        // prt2->vy -= v_n_y * 2 * prt1->mass / mass_sum * ELASTICITY;

        float j = (1 + cfg->elasticity) * scalar / (1/prt1->mass + 1/prt2->mass); // impulse

        prt1->vx += j / prt1->mass * nx;
        prt1->vy += j / prt1->mass * ny;
        prt2->vx -= j / prt2->mass * nx;
        prt2->vy -= j / prt2->mass * ny;


    }
}

// слияние
void merge(PhysicsContext *ctx, PhysicsConfig *cfg, int i, int j, float mass_sum, Particle* prt1, Particle* prt2) {
    // printf("Merge!\n");
    
    // средняя взвешенная скорость и позиция
    float new_vx = (prt1->mass * prt1->vx + prt2->mass * prt2->vx)  / mass_sum;
    float new_vy = (prt1->mass * prt1->vy + prt2->mass * prt2->vy)  / mass_sum;
    float new_x = (prt1->mass * prt1->x + prt2->mass * prt2->x)  / mass_sum;
    float new_y = (prt1->mass * prt1->y + prt2->mass * prt2->y)  / mass_sum;

    float new_radius = get_radius(mass_sum, cfg->density);

    prt1->mass = mass_sum;
    prt1->radius = new_radius;
    prt1->x = new_x;
    prt1->y = new_y;
    prt1->vx = new_vx;
    prt1->vy = new_vy;

    ctx->dead[j] = 1;
    // delete_particle(world, j);
}

void fix_overlap(World *world, PhysicsConfig *cfg, int i, int j, float dx, float dy, float dist, float mass_sum, float r, Particle* prt1, Particle* prt2) {
    // normal
    float nx = dx / dist;
    float ny = dy / dist;
    
    // overlap fixing
    float overlap = r - dist;
    printf("Overlap: %.10f \n", overlap);
    if (overlap > 0) {
        // prt1->x -= nx * overlap * prt2->mass / mass_sum * SEPARATE;
        // prt1->y -= ny * overlap * prt2->mass / mass_sum * SEPARATE;

        // // printf("Normal: %.1f; %.1f \n", nx, ny);
        // // printf("Moving: %.1f; %.1f \n", nx * overlap * prt2->mass / mass_sum, ny * overlap * prt2->mass / mass_sum);

        // prt2->x += nx * overlap * prt1->mass / mass_sum * SEPARATE;
        // prt2->y += ny * overlap * prt1->mass / mass_sum * SEPARATE;

        world->move_x[i] -= nx * overlap * prt2->mass / mass_sum * cfg->separate;
        world->move_y[i] -= ny * overlap * prt2->mass / mass_sum * cfg->separate;

        world->move_x[j] += nx * overlap * prt1->mass / mass_sum * cfg->separate;
        world->move_y[j] += ny * overlap * prt1->mass / mass_sum * cfg->separate;
    }
}

void handle_collision(PhysicsContext *ctx, PhysicsConfig *cfg, int i, int j) {
    Particle *prt1 = &ctx->world.particles[i];
    Particle *prt2 = &ctx->world.particles[j];
    float dx = prt2->x - prt1->x;
    float dy = prt2->y - prt1->y;

    float dist_sq = dx * dx + dy * dy;

    float r = (prt1->radius + prt2->radius);
    if (dist_sq <= r * r) { // on actual collision
        float dist = sqrtf(dist_sq);
        // printf("Collision! %d & %d\n", i, j);

        float v_rel_x = prt2->vx - prt1->vx;
        float v_rel_y = prt2->vy - prt1->vy;       
        
        float v_rel_sq = v_rel_x * v_rel_x + v_rel_y * v_rel_y;
        
        float mass_sum = prt1->mass + prt2->mass;

        float v_esc_sq = 2 * cfg->g * mass_sum / r;

        // printf("V_rel: %.1f; V_esc: %.1f;\n", v_rel_sq, v_esc_sq);
        if (v_rel_sq < cfg->col_alpha * v_esc_sq) {
            merge(ctx, cfg, i, j, mass_sum, prt1, prt2);
            // j--;
        } else if (v_rel_sq < cfg->col_beta * v_esc_sq) {
            bounce(cfg, i, j, dx, dy, dist, v_rel_x, v_rel_y, mass_sum, r, prt1, prt2);
        } else {
            bounce(cfg, i, j, dx, dy, dist, v_rel_x, v_rel_y, mass_sum, r, prt1, prt2); // shatter not yet implemented                    
        }
        
        // fix_overlap(&ctx->world, cfg, i, j, dx, dy, dist, mass_sum, r, prt1, prt2);
    }
}

