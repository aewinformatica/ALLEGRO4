#include <allegro.h>

#include "avf.h"

#include "power.h"



static unsigned char angle;
static int active;
static BITMAP *buffer = NULL;



static int e (BITMAP *bmp, fixed x, fixed y, fixed s, fixed angle, int color)
{
        fixed x2, y2;
        x2 = x + fixmul (s, fixcos (angle));
        y2 = y - fixmul (s, fixsin (angle));
        line (bmp, fixtoi (x), fixtoi (y), fixtoi (x2), fixtoi (y2), color);
}



static int A (BITMAP *bmp, fixed x, fixed y, fixed s, fixed angle, int iteration)
{
        int x1, y1, s1;
        if (iteration == 0)
                return;
        e (bmp, x, y, s, angle, iteration);
        x1 = x + fixmul (s, fixcos (angle));
        y1 = y - fixmul (s, fixsin (angle));
        s1 = fixdiv (s, ftofix (1.7));
        A (bmp, x1, y1, s1, angle - itofix (32), iteration - 1);
        A (bmp, x1, y1, s1, angle - itofix (4), iteration - 1);
}



static int S (BITMAP *bmp, fixed x, fixed y, fixed s, fixed angle, int iteration)
{
        int x1, y1, s1;
        if (iteration == 0)
                return;
        A (bmp, x, y, s, angle + itofix (8), iteration - 1);
        A (bmp, x, y, s, angle - itofix (4), iteration - 1);
        x1 = x + fixmul (s, fixcos (angle));
        y1 = y - fixmul (s, fixsin (angle));
        s1 = fixdiv (s, ftofix (1.09));
        S (bmp, x, y, s1, angle - itofix (32), iteration - 1);
        S (bmp, x, y, s1, angle - itofix (4), iteration - 1);
}



void init_power (void)
{
        if (buffer == NULL)
        {
                buffer = create_bitmap (200, 200);
                clear_bitmap (buffer);
                S (buffer, itofix (100), itofix (100), itofix (40), itofix (64), 12);
                angle = 0;
        }
        active = 0;
}



void activate_power (void)
{
        active = 1;
}



void deactivate_power (void)
{
        active = 0;
}



void draw_power (BITMAP *bmp, FIGHTER *player)
{
        if (!active || player->state != ST_SHOT)
                return;
        rotate_sprite (bmp, buffer, 0, 0, itofix (angle));
        angle++;
}



void end_power (void)
{
        if (buffer != NULL)
        {
                destroy_bitmap (buffer);
                buffer = NULL;
        }
}

