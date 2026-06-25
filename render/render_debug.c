#include <raylib.h>

#include "../physics/gravity/bh.h"
#include "../physics/physics_context.h"

void draw_bh_node(BH_Tree *tree, int node_i, int draw_occupied) {
    Node *node = &tree->nodes[node_i];

    // leaf
    if (node->children[0] == -1) {
        if (node->prt_index != -1 && draw_occupied)
            DrawRectangleLines(node->x - node->size * 0.5f, node->y - node->size * 0.5f, node->size, node->size, GREEN);
        else if (node->prt_index == -1 && !draw_occupied)
            DrawRectangleLines(node->x - node->size * 0.5f, node->y - node->size * 0.5f, node->size, node->size, GRAY);
        return;
    }

    // internal node
    // DrawRectangleLines(node->x - node->size * 0.5, node->y - node->size * 0.5, node->size, node->size, GRAY);
    for (int i = 0; i < 4; i++) {
        draw_bh_node(tree, node->children[i], draw_occupied);
    }
}

void visualise_bh_tree(PhysicsContext *ctx) {
    // build_bh_tree(ctx);
    draw_bh_node(&ctx->tree, 0, 0);
    draw_bh_node(&ctx->tree, 0, 1);
}

void visualise_grid(PhysicsContext *ctx) {
    for (int i = 0; i < ctx->grid.height; i++) {
        for (int j = 0; j < ctx->grid.width; j++) {
            float x = ctx->grid.root_x + j * ctx->grid.cell_size;
            float y = ctx->grid.root_y + i * ctx->grid.cell_size;
            DrawRectangleLines(x, y, ctx->grid.cell_size, ctx->grid.cell_size, GRAY);
        }
    }
}