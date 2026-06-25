#include <raylib.h>

#include "../core/world.h"

void render_world(World *world, float v_line_length) {
    for (int i = 0; i < world->p_count; i++) {
        Particle prtcl = world->particles[i];
        // DrawCircle(prtcl.x, prtcl.y, fmaxf(prtcl.radius, 1.0f / camera.zoom), WHITE);
        DrawCircle(prtcl.x, prtcl.y, prtcl.radius, WHITE);
        DrawLine(prtcl.x, prtcl.y, 
        prtcl.x + prtcl.vx * v_line_length, prtcl.y + prtcl.vy * v_line_length, GREEN);
    }
}