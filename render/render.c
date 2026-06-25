#include <math.h>
#include <raylib.h>
#include <stdlib.h>

#include "../ui/beton.h"

#include "../core/world.h"

void render_world(const World *world, Camera2D *camera, UIRect viewport, float v_line_length) {
    for (int i = 0; i < world->p_count; i++) {
        const Particle *prt = &world->particles[i];

        Vector2 screen = GetWorldToScreen2D((Vector2){prt->x, prt->y}, *camera);

        float offset = (fmaxf(fabsf(prt->vx), fabsf(prt->vy)) * v_line_length + prt->radius) * camera->zoom;
        if (screen.x < -offset || 
            screen.x > viewport.width + offset ||
            screen.y < -offset || 
            screen.y > viewport.height + offset) 
        {
            continue;
        }

        // DrawCircle(prt->x, prt->y, fmaxf(prt->radius, 1.0f / camera.zoom), WHITE);
        DrawCircle(prt->x, prt->y, prt->radius, WHITE);
        DrawLine(prt->x, prt->y, 
        prt->x + prt->vx * v_line_length, prt->y + prt->vy * v_line_length, GREEN);
    }
}