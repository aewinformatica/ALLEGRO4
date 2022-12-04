#include <allegro.h>
#include "explosion.h"
#include "timers.h"



#define POINTS_PER_PEAK  5
#define NUM_PEAKS        6
#define NUM_FACES        (4 * (NUM_PEAKS))
#define EXPLOSION_TIME   30
#define COLOR            80
#define Z_DISTANCE       50
#define INITIAL_SIZE     5


typedef struct POINT POINT;
typedef struct FACE FACE;
typedef struct OBJECT3D OBJECT3D;



struct POINT
{
        float x, y, z;
        float x2d, y2d;
};



struct FACE
{
        POINT a, b, c;
};



struct OBJECT3D
{
        int active;
        int time;
        float size;
        float rot_angle;
        int num_faces;
        FACE *faces;
};



static OBJECT3D explosion;
static MATRIX_f transform;
static FACE faces [NUM_FACES];



static POINT peaks [NUM_PEAKS][POINTS_PER_PEAK] =
{
        {
          { 0, -2, 0, 0, 0, }, // a
          { 0, -2, 2, 0, 0, }, // b
          { 2, -2, 0, 0, 0, }, // c
          { 2, -2, 2, 0, 0, }, // d
          { 1, -7, 1, 0, 0, }, // e
        },

        {
          { 0, 0, 0, 0, 0, }, // a
          { 0, 0, 2, 0, 0, }, // b
          { 2, 0, 0, 0, 0, }, // c
          { 2, 0, 2, 0, 0, }, // d
          { 1, 5, 1, 0, 0, }, // e
        },

        {
          {  0,  0, 0, 0, 0, }, // a
          {  0,  0, 2, 0, 0, }, // b
          {  0, -2, 0, 0, 0, }, // c
          {  0, -2, 2, 0, 0, }, // d
          { -5, -1, 1, 0, 0, }, // e
        },

        {
          { 2, -2, 0, 0, 0, }, // a
          { 2, -2, 2, 0, 0, }, // b
          { 2,  0, 0, 0, 0, }, // c
          { 2,  0, 2, 0, 0, }, // d
          { 7, -1, 1, 0, 0, }, // e
        },

        {
          { 0, -2, 2, 0, 0, }, // a
          { 0,  0, 2, 0, 0, }, // b
          { 2, -2, 2, 0, 0, }, // c
          { 2,  0, 2, 0, 0, }, // d
          { 1, -1, 7, 0, 0, }, // e
        },

        {
          { 0, -2,  0, 0, 0, }, // a
          { 0,  0,  0, 0, 0, }, // b
          { 2, -2,  0, 0, 0, }, // c
          { 2,  0,  0, 0, 0, }, // d
          { 1, -1, -5, 0, 0, }, // e
        },
};



static void assemble_explosion (void)
{
        int i, peak_index = 0;
        for (i = 0; i < NUM_FACES; i += 4)
        {
                faces [i].a = peaks [peak_index][0];
                faces [i].b = peaks [peak_index][4];
                faces [i].c = peaks [peak_index][1];

                faces [i + 1].a = peaks [peak_index][1];
                faces [i + 1].b = peaks [peak_index][4];
                faces [i + 1].c = peaks [peak_index][3];

                faces [i + 2].a = peaks [peak_index][3];
                faces [i + 2].b = peaks [peak_index][4];
                faces [i + 2].c = peaks [peak_index][2];

                faces [i + 3].a = peaks [peak_index][2];
                faces [i + 3].b = peaks [peak_index][4];
                faces [i + 3].c = peaks [peak_index][0];
                peak_index++;
        }
        explosion.num_faces = NUM_FACES;
        explosion.faces = faces;
        explosion.rot_angle = 0;
        explosion.size = INITIAL_SIZE;
        explosion.time = EXPLOSION_TIME;
        explosion.active = 1;
}



void start_explosion (int x, int y)
{
        if (explosion.active)
                return;
        assemble_explosion ();
        /* move the coords left to center the 3D rotation*/
        set_projection_viewport (x - 50, y, 100, 100);
}



static void project (POINT *p)
{
        float x, y, z;
        apply_matrix_f (&transform, p->x, p->y, p->z, &x, &y, &z);
        persp_project_f (x, y, z, &p->x2d, &p->y2d);
}



void update_explosion (void)
{
        MATRIX_f rot, trans, scale;
        int i;
        if (!explosion.active)
                return;
        get_translation_matrix_f (&trans, 0, 0, Z_DISTANCE);
        get_rotation_matrix_f (&rot, explosion.rot_angle, explosion.rot_angle, explosion.rot_angle);
        get_scaling_matrix_f (&scale, explosion.size, explosion.size, explosion.size);
        matrix_mul_f (&scale, &rot, &transform);
        matrix_mul_f (&transform, &trans, &transform);
        for (i = 0; i < explosion.num_faces; i++)
        {
                project (&explosion.faces [i].a);
                project (&explosion.faces [i].b);
                project (&explosion.faces [i].c);
        }
        explosion.size += explosion.time > (EXPLOSION_TIME >> 1) ? 0.2 : -0.2;
        explosion.rot_angle += 255 / EXPLOSION_TIME;
        explosion.time--;
        if (explosion.time < 0)
                end_explosion ();
}



static void draw_face (BITMAP *bmp, FACE f)
{
        line (bmp, f.a.x2d, f.a.y2d, f.b.x2d, f.b.y2d, COLOR);
        line (bmp, f.a.x2d, f.a.y2d, f.c.x2d, f.c.y2d, COLOR);
        line (bmp, f.b.x2d, f.b.y2d, f.c.x2d, f.c.y2d, COLOR);
}



void draw_explosion (BITMAP *bmp)
{
        int i;
        if (!explosion.active)
                return;
        for (i = 0; i < explosion.num_faces; i++)
                draw_face (bmp, explosion.faces [i]);
}



void end_explosion (void)
{
        explosion.active = 0;
}

