#pragma once

typedef struct World World;
typedef struct PhysicsConfig PhysicsConfig;
typedef struct PhysicsContext PhysicsContext;

typedef struct Node {
    float x, y;
    float size; // длина стороны
    float mass;
    float com_x, com_y;
    
    int prt_index;
    
    int children[4]; // -1 or index in array
} Node;

typedef struct BH_Tree {
    Node *nodes;
    int node_count;
} BH_Tree;

// void build_bh_tree(PhysicsContext *ctx);

void barnes_hut(PhysicsContext *ctx, PhysicsConfig *cfg, float *acc_x, float *acc_y);

void barnes_hut_parallel(PhysicsContext *ctx, PhysicsConfig *cfg, float *acc_x, float *acc_y);