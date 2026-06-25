#include <raylib.h>

#include "../ui/beton.h"

typedef struct World World;

void render_world(World *world, Camera2D *camera, UIRect viewport, float v_line_length);