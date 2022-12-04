#include <allegro.h>
#include "sprite.h"



#include "fighter.h"



int debug = 0;



void play_sprite (SPRITE *s)
{
        if (s->frame_timer > 0)
        {
                s->frame_timer--;
                return;
        }
        s->current_frame++;
        if (s->current_frame == s->anims [s->current_anim].length)
        {
                s->current_frame = s->anims [s->current_anim].first_frame;
                s->flags = SPRITE_DONE;
        }
        s->frame_timer = s->anims [s->current_anim].max_frame_time;
        s->image = (BITMAP *) s->data [s->current_anim].dat;
        debug = s->current_frame;
}



void reset_sprite (SPRITE *s)
{
        s->flags = 0;
        s->current_anim = s->default_anim;
        s->speed.x = 0;
        s->speed.y = 0;
        s->frame_timer = 0;
        s->current_frame = -1;
}



void move_sprite (SPRITE *s)
{
        s->pos.x += s->speed.x;
        s->pos.y -= s->speed.y;
        if (s->pos.x < 0) s->pos.x = 0;
        if (s->pos.y < 0) s->pos.y = 0;
        if (s->pos.x + s->size.x >= SCREEN_W) s->pos.x = SCREEN_W - s->size.x;
        if (s->pos.y + s->size.y >= SCREEN_H) s->pos.y = SCREEN_H - s->size.y;
}



int collision (SPRITE *s1, SPRITE *s2)
{
        BOX b1 = s1->bound;
        BOX b2 = s2->bound;
        if (b1.x1 > b2.x2 || b2.x1 > b1.x2 || b1.y1 > b2.y2 || b2.y1 > b1.y2)
                return 0;
        return 1;
}

