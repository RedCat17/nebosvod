#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../core/world.h"
#include "../../util/const.h"
#include "../physics_config.h"
#include "../physics_context.h"

#include "bh.h"


static int new_node(BH_Tree *tree, float x, float y, float size) {
    if (tree->node_count >= BH_MAX_NODES) {
        fprintf(stderr, "BH node pool exhausted!\n");
        exit(1);
    }
    int i = tree->node_count++;
    Node *node = &tree->nodes[i];

    node->x = x;
    node->y = y;
    node->size = size;
    node->mass = 0;
    node->com_x = x;
    node->com_y = y;

    node->prt_index = -1;
    
    node->children[0] = -1;
    node->children[1] = -1;
    node->children[2] = -1;
    node->children[3] = -1;

    return i; 
}

static void update_mass_com(Node *node, Particle *prt) {
    float old_mass = node->mass;
    float new_mass = old_mass + prt->mass;

    node->com_x = (node->com_x * node->mass + prt->mass * prt->x) / new_mass;
    node->com_y = (node->com_y * node->mass + prt->mass * prt->y) / new_mass;

    node->mass = new_mass;
}

static void put_in_node(BH_Tree *tree, int node_i, World *world, int prt_index) {
    Node *node = &tree->nodes[node_i];
    Particle *prt1 = &world->particles[prt_index];

    Rectangle tree_bounds = (Rectangle){
        tree->nodes[0].x - tree->nodes[0].size * 0.5f, 
        tree->nodes[0].y - tree->nodes[0].size * 0.5f, 
        tree->nodes[0].size, 
        tree->nodes[0].size};
    if (!CheckCollisionPointRec((Vector2){prt1->x, prt1->y}, tree_bounds)) {
        printf("Prt outside tree! node bounds: (%f, %f, %f, %f), prt: (%f; %f)\n", tree_bounds.x, tree_bounds.y, tree_bounds.x + tree_bounds.width, tree_bounds.y + tree_bounds.height, prt1->x, prt1->y);
    } 

    // update mass and CoM
    update_mass_com(node, &world->particles[prt_index]);

    // free leaf 
    if (node->prt_index == -1 && node->children[0] == -1) { 
        node->prt_index = prt_index;
        return;
    } 
    
    // occupied

    // if occupied leaf, subdivide
    if (node->children[0] == -1) {
        node->children[0] = new_node(tree, node->x - node->size * 0.25f, node->y - node->size * 0.25f, node->size * 0.5f); // nw
        node->children[1] = new_node(tree, node->x + node->size * 0.25f, node->y - node->size * 0.25f, node->size * 0.5f); // ne
        node->children[2] = new_node(tree, node->x - node->size * 0.25f, node->y + node->size * 0.25f, node->size * 0.5f); // sw
        node->children[3] = new_node(tree, node->x + node->size * 0.25f, node->y + node->size * 0.25f, node->size * 0.5f); // se

        // put both particles into correct children 
        Particle *prt2 = &world->particles[node->prt_index];
        if (node->size < 0.01f) printf("node size: %f, prt_o: (%f; %f), prt_n: (%f; %f)\n", node->size, prt1->x, prt1->y, prt2->x, prt2->y);
        if (prt2->y < node->y) {
            if (prt2->x < node->x) {
                put_in_node(tree, node->children[0], world, node->prt_index);
            } else {
                put_in_node(tree, node->children[1], world, node->prt_index);            
            } 
        } else {
            if (prt2->x < node->x) {
                put_in_node(tree, node->children[2], world, node->prt_index);
            } else {
                put_in_node(tree, node->children[3], world, node->prt_index);            
            } 
        }
    }

    // insert new particle into correct child
    if (prt1->y < node->y) {
        if (prt1->x < node->x) {
            put_in_node(tree, node->children[0], world, prt_index);
        } else {
            put_in_node(tree, node->children[1], world, prt_index);            
        } 
    } else {
        if (prt1->x < node->x) {
            put_in_node(tree, node->children[2], world, prt_index);
        } else {
            put_in_node(tree, node->children[3], world, prt_index);            
        } 
    }

    node->prt_index = -1;
}

static void print_node(BH_Tree *tree, int node_i, int depth) {
    Node *node = &tree->nodes[node_i];
    for (int i = 0; i < depth; i++ ) printf(" ");
    printf("node %d | x: %.2f, y: %.2f, size: %.2f, mass: %.2f, com_x: %.2f, com_y: %.2f", node_i, node->x, node->y, node->size, node->mass, node->com_x, node->com_y);
    
    if (node->children[0] == -1) {
        printf(", leaf");
        if (node->prt_index != -1) {
            printf(", prt: %d", node->prt_index);
        }
        printf("\n");
        return;
    }
    printf("\n");

    for (int i = 0; i < 4; i++) {
        print_node(tree, node->children[i], depth + 1);
    }
}

void print_tree(BH_Tree *tree) {
    printf("BH tree:\n");

    print_node(tree, 0, 0);
}

static void build_bh_tree(PhysicsContext *ctx) {
    ctx->tree.node_count = 0;

    float cx = (ctx->world.minX + ctx->world.maxX) * 0.5f, cy = (ctx->world.minY + ctx->world.maxY) * 0.5f;
    float size = fmaxf(ctx->world.maxX - ctx->world.minX, ctx->world.maxY - ctx->world.minY) * 1.1f;

    // printf("Tree root: x=%.2f, y=%.2f, size=%.2f\n", cx, cy, size);

    // building a tree
    new_node(&ctx->tree, cx, cy, size);
    // new_node(tree, 0, 0, BH_ROOT_SIZE);

    for (int i = 0; i < ctx->world.p_count; i++) {
        put_in_node(&ctx->tree, 0, &ctx->world, i);
    }
}

static void compute_force_to_node_recursive(Particle *prt, PhysicsConfig *cfg, int prt_index, int node_i, BH_Tree *tree, float theta, float *acc_x, float *acc_y) {
    // printf("force from prt %d to node %d\n", prt_index, node_i);
    Node *node = &tree->nodes[node_i];

    float dx = node->com_x - prt->x;
    float dy = node->com_y - prt->y;

    float dist_sq = dx * dx + dy * dy + cfg->eps * cfg->eps;
    float dist = sqrtf(dist_sq);
    // case 1: leaf or far node
    // if ((node->children[0] == -1) || ((node->size * node->size / dist_sq) < (theta * theta))) {    
    if ((node->children[0] == -1) || ((node->size / dist) < theta)) {        
        if (node->prt_index == prt_index) {
            return;
        }
        float inv_dist = 1.0f / dist;

        float acc = cfg->g * node->mass * inv_dist * inv_dist;

        *acc_x += acc * dx * inv_dist;
        *acc_y += acc * dy * inv_dist;
        return;
    }  
    
    // case 2: close internal node
    // printf("subdividing...\n");
    for (int i = 0; i < 4; i++) {
        compute_force_to_node_recursive(prt, cfg, prt_index, node->children[i], tree, theta, acc_x, acc_y);
    }
}

static void compute_force_to_node(Particle *prt, PhysicsConfig *cfg, int prt_index, BH_Tree *tree, float theta, float *acc_x, float *acc_y) {
    // printf("force from prt %d to node %d\n", prt_index, node_i);
    int stack[MAX_STACK];
    int top = 0;

    stack[top++] = 0; // root

    while (top > 0) {
        Node *node = &tree->nodes[stack[--top]];

        float dx = node->com_x - prt->x;
        float dy = node->com_y - prt->y;

        float dist_sq = dx * dx + dy * dy + cfg->eps * cfg->eps;
        // float dist = sqrtf(dist_sq);
        // case 1: leaf or far node
        if ((node->children[0] == -1) || ((node->size * node->size) < (theta * theta * dist_sq))) {    
        // if ((node->children[0] == -1) || ((node->size / dist) < theta)) {        
            if (node->prt_index == prt_index) {
                continue;
            }
            float dist = sqrtf(dist_sq);
            float inv_dist = 1.0f / dist;

            float acc = cfg->g * node->mass * inv_dist * inv_dist;

            *acc_x += acc * dx * inv_dist;
            *acc_y += acc * dy * inv_dist;
        } else {
            // case 2: close internal node
            for (int i = 0; i < 4; i++) {
                if (top >= MAX_STACK) {
                    fprintf(stderr, "Stack overflow!\n");
                    exit(1);
                }
                stack[top++] = node->children[i];
            }
        }
        
        
    }    
}

void barnes_hut(PhysicsContext *ctx, PhysicsConfig *cfg, float *acc_x, float *acc_y) {
    memset(acc_x, 0, sizeof(float) * ctx->world.p_count);
    memset(acc_y, 0, sizeof(float) * ctx->world.p_count);
    
    build_bh_tree(ctx);

    // debug printing tree
    // print_tree( &tree);

    for (int i = 0; i < ctx->world.p_count; i++) {
        Particle *prt = &ctx->world.particles[i];
        // compute_force_to_node_recursive(prt, i, 0, &world->tree, BH_THETA, &acc_x[i], &acc_y[i]);
        compute_force_to_node(prt, cfg, i, &ctx->tree, cfg->bh_theta, &acc_x[i], &acc_y[i]);
    } 
}

typedef struct {
    PhysicsContext *ctx;
    PhysicsConfig *cfg;
    int start;
    int end;
    float *acc_x;
    float *acc_y;
} WorkerArgs;

static void* worker_force(void *arg) {
    WorkerArgs *a = (WorkerArgs*)arg; // casting arg (void) to WorkerArgs to use it
    
    for (int i = a->start; i < a->end; i++) {
        Particle *prt = &a->ctx->world.particles[i];
        // compute_force_to_node_recursive(prt, i, 0, &world->tree, BH_THETA, &acc_x[i], &acc_y[i]);
        compute_force_to_node(prt, a->cfg, i, &a->ctx->tree, a->cfg->bh_theta, &a->acc_x[i], &a->acc_y[i]);
    } 

    return NULL;
}

void barnes_hut_parallel(PhysicsContext *ctx, PhysicsConfig *cfg, float *acc_x, float *acc_y) {
    memset(acc_x, 0, sizeof(float) * ctx->world.p_count);
    memset(acc_y, 0, sizeof(float) * ctx->world.p_count);
    
    build_bh_tree(ctx);
    // debug printing tree
    // print_tree( &tree);

    pthread_t threads[THREADS];
    WorkerArgs args[THREADS];

    int chunk = ctx->world.p_count / THREADS;

    for (int t = 0; t < THREADS; t++) {
        args[t] = (WorkerArgs){
            .ctx = ctx,
            .cfg = cfg,
            .start = t * chunk,
            .end = (t == THREADS - 1)? ctx->world.p_count : (t + 1) * chunk,
            .acc_x = acc_x,
            .acc_y = acc_y
        };

        pthread_create(&threads[t], NULL, worker_force, &args[t]);
    }

    for (int t = 0; t < THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

}