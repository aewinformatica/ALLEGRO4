/*
 * actor.c - actor module
 * Copyright (C) 2008-2009  Alexandre Martins < alemartf [.a.t.] gmail [.d.o.t.] com >
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#include <allegro.h>
#include <math.h>
#include "global.h"
#include "actor.h"
#include "video.h"
#include "input.h"
#include "level.h"


/* private data */
static int floor_priority = TRUE; /* default behavior: priority(floor) > priority(wall) */
static int slope_priority = TRUE; /* default behavior: priority(slope) > priority(floor) */
static brick_t* brick_at(brick_list_t *list, double rect[4]);


/* actor functions */



/*
 * actor_create()
 * Creates an actor
 */
actor_t* actor_create()
{
    actor_t *act = mallocx(sizeof *act);

    act->spawn_point = v2d_new(0,0);
    act->position = act->spawn_point;
    act->angle = 0;
    act->speed = v2d_new(0,0);
    act->maxspeed = 0;
    act->acceleration = 0;
    act->jump_strength = 0;
    act->is_jumping = FALSE;
    act->ignore_horizontal = FALSE;
    act->input = NULL;

    act->animation = NULL;
    act->animation_frame = 0;
    act->mirror = IF_NONE;
    act->visible = TRUE;
    act->hot_spot = v2d_new(0,0);

    return act;
}


/*
 * actor_destroy()
 * Destroys an actor
 */
void actor_destroy(actor_t *act)
{
    if(act->input)
        input_destroy(act->input);
    free(act);
}


/*
 * actor_render()
 * Default rendering function
 */
void actor_render(actor_t *act, v2d_t camera_position)
{
    double diff = -2;
    image_t *img;
    v2d_t tmp;

    if(act->visible && act->animation) {
        /* update animation */
        act->animation_frame += act->animation->fps * game_delta();
        if((int)act->animation_frame >= act->animation->frame_count) {
            if(act->animation->repeat)
                act->animation_frame = (int)act->animation_frame % act->animation->frame_count;
            else
                act->animation_frame = act->animation->frame_count-1;
        }

        /* render */
        tmp = act->position;
        img = actor_image(act);
        actor_move(act, v2d_new(0,-diff));
        if(fabs(act->angle) > EPSILON)
           image_draw_rotated(img, video_buffer, (int)(act->position.x-(camera_position.x-VIDEO_SCREEN_W/2)), (int)(act->position.y-(camera_position.y-VIDEO_SCREEN_H/2)), (int)act->hot_spot.x, (int)act->hot_spot.y, act->angle, act->mirror);
        else
           image_draw(img, video_buffer, (int)(act->position.x-act->hot_spot.x-(camera_position.x-VIDEO_SCREEN_W/2)), (int)(act->position.y-act->hot_spot.y-(camera_position.y-VIDEO_SCREEN_H/2)), act->mirror);
        act->position = tmp;

        /* show collision detectors (debug) */
        /*double top = 1.0, middle = 0.7, lateral = 0.25;
        double frame_width=actor_image(act)->w, frame_height=actor_image(act)->h;
        double offx=camera_position.x-VIDEO_SCREEN_W/2;
        double offy=camera_position.y-VIDEO_SCREEN_H/2;
        v2d_t feet      = act->position;
        v2d_t up        = v2d_add ( feet , v2d_rotate( v2d_new(0, -frame_height*top+diff), -act->angle) );
        v2d_t down      = v2d_add ( feet , v2d_rotate( v2d_new(0, -diff), -act->angle) ); 
        v2d_t left      = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -frame_height*middle), -act->angle) );
        v2d_t right     = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -frame_height*middle), -act->angle) );
        v2d_t upleft    = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -frame_height*top+diff), -act->angle) );
        v2d_t upright   = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -frame_height*top+diff), -act->angle) );
        v2d_t downleft  = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -diff), -act->angle) );
        v2d_t downright = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -diff), -act->angle) );
        double sqrsize = 2;
        double cd_up[4] = { up.x-sqrsize-offx , up.y-sqrsize-offy , up.x+sqrsize-offx , up.y+sqrsize-offy };
        double cd_down[4] = { down.x-sqrsize-offx , down.y-sqrsize-offy , down.x+sqrsize-offx , down.y+sqrsize-offy };
        double cd_left[4] = { left.x-sqrsize-offx , left.y-sqrsize-offy , left.x+sqrsize-offx , left.y+sqrsize-offy };
        double cd_right[4] = { right.x-sqrsize-offx , right.y-sqrsize-offy , right.x+sqrsize-offx , right.y+sqrsize-offy };
        double cd_downleft[4] = { downleft.x-sqrsize-offx , downleft.y-sqrsize-offy , downleft.x+sqrsize-offx , downleft.y+sqrsize-offy };
        double cd_downright[4] = { downright.x-sqrsize-offx , downright.y-sqrsize-offy , downright.x+sqrsize-offx , downright.y+sqrsize-offy };
        double cd_upright[4] = { upright.x-sqrsize-offx , upright.y-sqrsize-offy , upright.x+sqrsize-offx , upright.y+sqrsize-offy };
        double cd_upleft[4] = { upleft.x-sqrsize-offx , upleft.y-sqrsize-offy , upleft.x+sqrsize-offx , upleft.y+sqrsize-offy };
        rectfill(video_buffer->data, cd_up[0], cd_up[1], cd_up[2], cd_up[3], 0xFFFFFF);
        rectfill(video_buffer->data, cd_down[0], cd_down[1], cd_down[2], cd_down[3], 0xFFFFFF);
        rectfill(video_buffer->data, cd_left[0], cd_left[1], cd_left[2], cd_left[3], 0xFFFFFF);
        rectfill(video_buffer->data, cd_right[0], cd_right[1], cd_right[2], cd_right[3], 0xFFFFFF);
        rectfill(video_buffer->data, cd_downleft[0], cd_downleft[1], cd_downleft[2], cd_downleft[3], random(0xFFFFFF));
        rectfill(video_buffer->data, cd_downright[0], cd_downright[1], cd_downright[2], cd_downright[3], random(0xFFFFFF));
        rectfill(video_buffer->data, cd_upright[0], cd_upright[1], cd_upright[2], cd_upright[3], random(0xFFFFFF));
        rectfill(video_buffer->data, cd_upleft[0], cd_upleft[1], cd_upleft[2], cd_upleft[3], random(0xFFFFFF));*/
        
    }



}



/*
 * actor_render_repeat_xy()
 * Rendering / repeat xy
 */
void actor_render_repeat_xy(actor_t *act, v2d_t camera_position, int repeat_x, int repeat_y)
{
    int i, j, w, h;
    image_t *img = actor_image(act);
    v2d_t final_pos;

    final_pos.x = (int)act->position.x%(repeat_x?img->w:INT_MAX) - act->hot_spot.x-(camera_position.x-VIDEO_SCREEN_W/2) - (repeat_x?img->w:0);
    final_pos.y = (int)act->position.y%(repeat_y?img->h:INT_MAX) - act->hot_spot.y-(camera_position.y-VIDEO_SCREEN_H/2) - (repeat_y?img->h:0);

    if(act->visible && act->animation) {
        /* update animation */
        act->animation_frame += act->animation->fps * game_delta();
        if((int)act->animation_frame >= act->animation->frame_count) {
            if(act->animation->repeat)
                act->animation_frame = (int)act->animation_frame % act->animation->frame_count;
            else
                act->animation_frame = act->animation->frame_count-1;
        }

        /* render */
        w = repeat_x ? (VIDEO_SCREEN_W/img->w + 3) : 1;
        h = repeat_y ? (VIDEO_SCREEN_H/img->h + 3) : 1;
        for(i=0; i<w; i++) {
            for(j=0; j<h; j++)
                image_draw(img, video_buffer, (int)final_pos.x + i*img->w, (int)final_pos.y + j*img->h, act->mirror);
        }
    }
}




/*
 * actor_collision()
 * Check collisions
 */
int actor_collision(actor_t *a, actor_t *b)
{
    int j, right = 0;
    v2d_t corner[2][4];
    corner[0][0] = v2d_subtract(a->position, v2d_rotate(a->hot_spot, -a->angle)); /* a's topleft */
    corner[0][1] = v2d_add( corner[0][0] , v2d_rotate(v2d_new(actor_image(a)->w, 0), -a->angle) ); /* a's topright */
    corner[0][2] = v2d_add( corner[0][0] , v2d_rotate(v2d_new(actor_image(a)->w, actor_image(a)->h), -a->angle) ); /* a's bottomright */
    corner[0][3] = v2d_add( corner[0][0] , v2d_rotate(v2d_new(0, actor_image(a)->h), -a->angle) ); /* a's bottomleft */
    corner[1][0] = v2d_subtract(b->position, v2d_rotate(b->hot_spot, -b->angle)); /* b's topleft */
    corner[1][1] = v2d_add( corner[1][0] , v2d_rotate(v2d_new(actor_image(b)->w, 0), -b->angle) ); /* b's topright */
    corner[1][2] = v2d_add( corner[1][0] , v2d_rotate(v2d_new(actor_image(b)->w, actor_image(b)->h), -b->angle) ); /* b's bottomright */
    corner[1][3] = v2d_add( corner[1][0] , v2d_rotate(v2d_new(0, actor_image(b)->h), -b->angle) ); /* b's bottomleft */
    right += fabs(a->angle)<EPSILON||fabs(a->angle-PI/2)<EPSILON||fabs(a->angle-PI)<EPSILON||fabs(a->angle-3*PI/2)<EPSILON;
    right += fabs(b->angle)<EPSILON||fabs(b->angle-PI/2)<EPSILON||fabs(b->angle-PI)<EPSILON||fabs(b->angle-3*PI/2)<EPSILON;

    if(right) {
        double r[2][4];
        for(j=0; j<2; j++) {
            r[j][0] = min(corner[j][0].x, corner[j][1].x);
            r[j][1] = min(corner[j][0].y, corner[j][1].y);
            r[j][2] = max(corner[j][2].x, corner[j][3].x);
            r[j][3] = max(corner[j][2].y, corner[j][3].y);
            if(r[j][0] > r[j][2]) swap(r[j][0], r[j][2]);
            if(r[j][1] > r[j][3]) swap(r[j][1], r[j][3]);
        }
        return bounding_box(r[0],r[1]);
    }
    else {
        v2d_t center[2];
        double radius[2] = { max(actor_image(a)->w,actor_image(a)->h) , max(actor_image(b)->w,actor_image(b)->h) };
        for(j=0; j<2; j++)
            center[j] = v2d_multiply(v2d_add(corner[j][0], corner[j][2]), 0.5);
        return circular_collision(center[0], radius[0], center[1], radius[1]);
    }
}


/*
 * actor_brick_collision()
 * Actor collided with a brick?
 */
int actor_brick_collision(actor_t *act, brick_t *brk)
{
    v2d_t topleft = v2d_subtract(act->position, v2d_rotate(act->hot_spot, act->angle));
    v2d_t bottomright = v2d_add( topleft , v2d_rotate(v2d_new(actor_image(act)->w, actor_image(act)->h), act->angle) );
    double a[4] = { topleft.x , topleft.y , bottomright.x , bottomright.y };
    double b[4] = { (double)brk->x , (double)brk->y , (double)(brk->x+brk->brick_ref->image->w) , (double)(brk->y+brk->brick_ref->image->h) };

    return bounding_box(a,b);
}



/*
 * actor_move()
 * Uses the orientation angle to move an actor
 */
void actor_move(actor_t *act, v2d_t delta_space)
{
    if(fabs(delta_space.x) < EPSILON) delta_space.x = 0;
    act->position.x += delta_space.x * cos(act->angle) + delta_space.y * sin(act->angle);
    act->position.y += delta_space.y * cos(act->angle) - delta_space.x * sin(act->angle);
}


/*
 * actor_change_animation()
 * Changes the animation of an actor
 */
void actor_change_animation(actor_t *act, animation_t *anim)
{
    if(act->animation != anim) {
        act->animation = anim;
        act->hot_spot = anim->hot_spot;
        act->animation_frame = 0;
    }
}



/*
 * actor_change_animation_frame()
 * Changes the animation frame
 */
void actor_change_animation_frame(actor_t *act, int frame)
{
    act->animation_frame = clip(frame, 0, act->animation->frame_count);
}



/*
 * actor_animation_finished()
 * Returns true if the current animation has finished
 */
int actor_animation_finished(actor_t *act)
{
    double frame = act->animation_frame + act->animation->fps*game_delta();
    return (!act->animation->repeat &&
            (int)frame >= act->animation->frame_count);
}



/*
 * actor_image()
 * Returns the current image of the
 * animation of this actor
 */
image_t* actor_image(actor_t *act)
{
    return sprite_get_image(act->animation, (int)act->animation_frame);
}






/*
 * actor_corners()
 * Get actor's corners
 *
 * Let X be the original collision detector spot. So:
 * diff = distance between the real collision detector spot and X
 * sqrsize = size of the collision detector
 */
void actor_corners(actor_t *act, double sqrsize, double diff, brick_list_t *brick_list, brick_t **up, brick_t **upright, brick_t **right, brick_t **downright, brick_t **down, brick_t **downleft, brick_t **left, brick_t **upleft)
{
    int frame_width = actor_image(act)->w;
    int frame_height = actor_image(act)->h;

    v2d_t feet      = act->position;
    v2d_t vup        = v2d_add ( feet , v2d_rotate( v2d_new(0, -frame_height+diff), -act->angle) );
    v2d_t vdown      = v2d_add ( feet , v2d_rotate( v2d_new(0, -diff), -act->angle) ); 
    v2d_t vleft      = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width/2+diff, -frame_height*0.8), -act->angle) );
    v2d_t vright     = v2d_add ( feet , v2d_rotate( v2d_new(frame_width/2-diff, -frame_height*0.8), -act->angle) );
    v2d_t vupleft    = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width/2+diff, -frame_height+diff), -act->angle) );
    v2d_t vupright   = v2d_add ( feet , v2d_rotate( v2d_new(frame_width/2-diff, -frame_height+diff), -act->angle) );
    v2d_t vdownleft  = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width/2+diff, -diff), -act->angle) );
    v2d_t vdownright = v2d_add ( feet , v2d_rotate( v2d_new(frame_width/2-diff, -diff), -act->angle) );

    actor_corners_ex(act, sqrsize, vup, vupright, vright, vdownright, vdown, vdownleft, vleft, vupleft, brick_list, up, upright, right, downright, down, downleft, left, upleft);
}




/*
 * actor_corners_ex()
 * Like actor_corners(), but this procedure allows to specify the
 * collision detectors positions'
 */
void actor_corners_ex(actor_t *act, double sqrsize, v2d_t vup, v2d_t vupright, v2d_t vright, v2d_t vdownright, v2d_t vdown, v2d_t vdownleft, v2d_t vleft, v2d_t vupleft, brick_list_t *brick_list, brick_t **up, brick_t **upright, brick_t **right, brick_t **downright, brick_t **down, brick_t **downleft, brick_t **left, brick_t **upleft)
{
    double cd_up[4] = { vup.x-sqrsize , vup.y-sqrsize , vup.x+sqrsize , vup.y+sqrsize };
    double cd_down[4] = { vdown.x-sqrsize , vdown.y-sqrsize , vdown.x+sqrsize , vdown.y+sqrsize };
    double cd_left[4] = { vleft.x-sqrsize , vleft.y-sqrsize , vleft.x+sqrsize , vleft.y+sqrsize };
    double cd_right[4] = { vright.x-sqrsize , vright.y-sqrsize , vright.x+sqrsize , vright.y+sqrsize };
    double cd_upleft[4] = { vupleft.x-sqrsize , vupleft.y-sqrsize , vupleft.x+sqrsize , vupleft.y+sqrsize };
    double cd_upright[4] = { vupright.x-sqrsize , vupright.y-sqrsize , vupright.x+sqrsize , vupright.y+sqrsize };
    double cd_downleft[4] = { vdownleft.x-sqrsize , vdownleft.y-sqrsize , vdownleft.x+sqrsize , vdownleft.y+sqrsize };
    double cd_downright[4] = { vdownright.x-sqrsize , vdownright.y-sqrsize , vdownright.x+sqrsize , vdownright.y+sqrsize };

    if(up) *up = brick_at(brick_list, cd_up);
    if(down) *down = brick_at(brick_list, cd_down);
    if(left) *left = brick_at(brick_list, cd_left);
    if(right) *right = brick_at(brick_list, cd_right);
    if(upleft) *upleft = brick_at(brick_list, cd_upleft);
    if(upright) *upright = brick_at(brick_list, cd_upright);
    if(downleft) *downleft = brick_at(brick_list, cd_downleft);
    if(downright) *downright = brick_at(brick_list, cd_downright);
}


/*
 * actor_corners_set_floor_priority()
 * Which one has the greatest priority: floor (TRUE) or wall (FALSE) ?
 * Collision-detection routine. See also: brick_at()
 */
void actor_corners_set_floor_priority(int floor)
{
    floor_priority = floor;
}


/*
 * actor_corners_restore_floor_priority()
 * Shortcut to actor_corners_set_floor_priority(TRUE);
 * TRUE is the default value.
 */
void actor_corners_restore_floor_priority()
{
    actor_corners_set_floor_priority(TRUE);
}

/*
 * actor_corners_set_slope_priority()
 * Which one has the greatest priority: slope (TRUE) or floor (FALSE) ?
 * Collision-detection routine. See also: brick_at()
 */
void actor_corners_set_slope_priority(int slope)
{
    slope_priority = slope;
}


/*
 * actor_corners_restore_slope_priority()
 * Shortcut to actor_corners_set_slope_priority(TRUE);
 * TRUE is the default value.
 */
void actor_corners_restore_slope_priority()
{
    actor_corners_set_slope_priority(TRUE);
}


/*
 * actor_platform_movement()
 * Basic platform movement. Returns
 * a delta_space vector.
 *
 * Note: the actor's hot spot must
 * be defined on its feet.
 */
v2d_t actor_platform_movement(actor_t *act, brick_list_t *brick_list, double gravity)
{
    double dt = game_delta();
    double max_y_speed = 480, friction = 0, gravity_factor = 1.0;
    double desired_angle = 0;
    v2d_t ds = v2d_new(0,0);
    int pushing_a_wall;

    /* actor's collision detectors */
    int frame_width = actor_image(act)->w, frame_height = actor_image(act)->h;
    int slope = !((fabs(act->angle)<EPSILON)||(fabs(act->angle-PI/2)<EPSILON)||(fabs(act->angle-PI)<EPSILON)||(fabs(act->angle-3*PI/2)<EPSILON));
    double diff = -2, sqrsize = 2, top=0, middle=0, lateral=0;
    brick_t *brick_up, *brick_down, *brick_right, *brick_left;
    brick_t *brick_upright, *brick_downright, *brick_downleft, *brick_upleft;
    brick_t *brick_tmp;
    v2d_t up, upright, right, downright, down, downleft, left, upleft;
    v2d_t feet = act->position;
    if(!slope) { top = 0.7; middle = 0.5; lateral = 0.25; }
    else       { top = 1.0; middle = 0.7; lateral = 0.25; }
    up        = v2d_add ( feet , v2d_rotate( v2d_new(0, -frame_height*top+diff), -act->angle) );
    down      = v2d_add ( feet , v2d_rotate( v2d_new(0, -diff), -act->angle) ); 
    left      = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -frame_height*middle), -act->angle) );
    right     = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -frame_height*middle), -act->angle) );
    upleft    = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -frame_height*top+diff), -act->angle) );
    upright   = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -frame_height*top+diff), -act->angle) );
    downleft  = v2d_add ( feet , v2d_rotate( v2d_new(-frame_width*lateral+diff, -diff), -act->angle) );
    downright = v2d_add ( feet , v2d_rotate( v2d_new(frame_width*lateral-diff, -diff), -act->angle) );
    actor_corners_ex(act, sqrsize, up, upright, right, downright, down, downleft, left, upleft, brick_list, &brick_up, &brick_upright, &brick_right, &brick_downright, &brick_down, &brick_downleft, &brick_left, &brick_upleft);


    /* useful flags */
    pushing_a_wall = ((brick_right&&input_button_down(act->input, IB_RIGHT)) || (brick_left&&input_button_down(act->input, IB_LEFT))) && brick_down;



    /* wall collision */
    if(1||!act->is_jumping){ /* <-- test me */
        if(brick_right) {
            if(brick_right->brick_ref->angle % 90 == 0 && (act->speed.x > EPSILON || right.x > brick_right->x)) {
                act->speed.x = 0;
                act->position.x = brick_right->x + (feet.x-right.x);
            }
        }

        if(brick_left) {
            if(brick_left->brick_ref->angle % 90 == 0 && (act->speed.x < -EPSILON || left.x < brick_left->x+brick_left->brick_ref->image->w)) {
                act->speed.x = 0;
                act->position.x = (brick_left->x+brick_left->brick_ref->image->w) + (feet.x-left.x);
            }
        }
    }



    /* y-axis */
    if(brick_down) {
        int ang = brick_down->brick_ref->angle;
        double factor;
        act->is_jumping = FALSE;
        act->ignore_horizontal = FALSE;

        /* jump */
        if(input_button_down(act->input, IB_FIRE1) && !input_button_down(act->input, IB_DOWN) && !brick_up) {
            act->angle = desired_angle;
            act->is_jumping = TRUE;
            if(ang == 0) {
                act->speed.y = -act->jump_strength;
            }
            else if(ang > 0 && ang < 90) {
                act->speed.x = min(act->speed.x, -0.7*act->jump_strength);
                act->speed.y = -0.7*act->jump_strength;
            }
            else if(ang == 90) {
                actor_move(act, v2d_new(20*diff, 0));
                act->speed.x = min(act->speed.x, -act->jump_strength);
                act->speed.y = -act->jump_strength/2;
            }
            else if(ang > 90 && ang < 180) {
                actor_move(act, v2d_new(0, -20*diff));
                act->speed.x = min(act->speed.x, -0.7*act->jump_strength);
                act->speed.y = act->jump_strength;
            }
            else if(ang == 180) {
                actor_move(act, v2d_new(0, -20*diff));
                act->speed.x *= -1;
                act->speed.y = act->jump_strength;
            }
            else if(ang > 180 && ang < 270) {
                actor_move(act, v2d_new(0, -20*diff));
                act->speed.x = max(act->speed.x, 0.7*act->jump_strength);
                act->speed.y = act->jump_strength;
            }
            else if(ang == 270) {
                actor_move(act, v2d_new(-20*diff, 0));
                act->speed.x = max(act->speed.x, act->jump_strength);
                act->speed.y = -act->jump_strength/2;
            }
            else if(ang > 270 && ang < 360) {
                act->speed.x = max(act->speed.x, 0.7*act->jump_strength);
                act->speed.y = -0.7*act->jump_strength;
            }
        }

        /* slopes / speed issues */
        if(!act->is_jumping) {
            double mytan;
            if(ang > 0 && ang < 90) {
                mytan = min(1, tan( ang*PI/180.0 ));
                if(fabs(act->speed.y) > EPSILON)
                    act->speed.x = -3*mytan*act->speed.y;
                else {
                    factor = (!(act->mirror & IF_HFLIP) ? 0.8 : 2.0) * mytan;
                    act->speed.x = max(act->speed.x - factor*act->acceleration*dt, -act->maxspeed);
                }
            }
            else if(ang > 270 && ang < 360) {
                mytan = min(1, -tan( ang*PI/180.0 ));
                if(fabs(act->speed.y) > EPSILON)
                    act->speed.x = 3*mytan*act->speed.y;
                else {
                    factor = ((act->mirror & IF_HFLIP) ? 0.8 : 2.0) * mytan;
                    act->speed.x = min(act->speed.x + factor*act->acceleration*dt, act->maxspeed);
                }
            }
        }
    }
    else { /* not brick_down */
        act->angle = desired_angle;
    }


    /* y-axis movement */
    gravity_factor = 1.0;
    ds.y = (fabs(act->speed.y) > EPSILON) ? act->speed.y*dt + 0.5*(gravity*gravity_factor)*(dt*dt) : 0;
    act->speed.y = min(act->speed.y + (gravity*gravity_factor)*dt, max_y_speed);




    /* ceil collision */
    if(brick_up && brick_up->brick_ref->angle % 90 == 0 && act->speed.y < -EPSILON) {
        act->position.y = (brick_up->y+brick_up->brick_ref->image->h) + (feet.y-up.y);
        act->speed.y = 10;
    }



    /* floor collision */
    brick_tmp = brick_down;
    if(brick_tmp && !act->is_jumping) {

        int ang = brick_tmp->brick_ref->angle;
        act->speed.y = ds.y = 0;
        act->angle = ang * PI / 180.0;

        /* 0 floor */
        if(ang == 0) {
            v2d_t mov = level_brick_move_actor(brick_down, act); /* moveable platforms I */
            feet.y = brick_tmp->y;
            friction = 0;
            if(mov.y > EPSILON) /* if the moveable brick is going down... */
                ds.y += mov.y*dt;
            else
                act->position.y = feet.y+diff;
        }

        /* (0-90) slope */
        else if(ang > 0 && ang < 90) {
            feet.y = brick_tmp->y + brick_tmp->brick_ref->image->h - (act->position.x-brick_tmp->x)*tan(act->angle);
            act->position.y = feet.y+diff;
            if(!(act->mirror & IF_HFLIP)) friction = 0.2;
        }

        /* 90 wall */
        else if(ang == 90) {
            if(fabs(act->speed.x) > 5) {
                int myang = brick_downright ? brick_downright->brick_ref->angle : -1;
                if(brick_downright && (myang >= ang && myang < ang+90)) {
                    feet.y = brick_tmp->x;
                    act->position.x = feet.y+diff;
                }
                else {
                    act->angle = desired_angle;
                    actor_move(act, v2d_new(6.5*diff, 0));
                    act->is_jumping = TRUE;
                    act->speed = v2d_new(0, -0.7*fabs(act->speed.x));
                }
            }
            else {
                act->angle = desired_angle;
                actor_move(act, v2d_new(5*diff, 0));
                act->is_jumping = TRUE;
                act->ignore_horizontal = FALSE;
            }
            if(!(act->mirror & IF_HFLIP)) friction = 1.5;
        }

        /* (90-180) slope */
        else if(ang > 90 && ang < 180) {
            if(fabs(act->speed.x) > 5) {
                feet.y = brick_tmp->y - (act->position.x-brick_tmp->x)*tan(act->angle);
                act->position.y = feet.y-diff;
            }
            else {
                act->angle = desired_angle;
                actor_move(act, v2d_new(0, -15*diff));
                act->is_jumping = TRUE;
            }
            friction = 1.5;
        }

        /* 180 ceil */
        else if(ang == 180) {
            if( (act->speed.x > 5 && !(act->mirror & IF_HFLIP)) || (act->speed.x < -5 && act->mirror & IF_HFLIP) ) {
                feet.y = brick_tmp->y + brick_tmp->brick_ref->image->h;
                act->position.y = feet.y-diff;
            }
            else {
                act->angle = desired_angle;
                actor_move(act, v2d_new(0, -20*diff));
                act->is_jumping = TRUE;
                act->speed.x = 0;
            }
            friction = 1.2;
        }

        /* (180-270) slope */
        else if(ang > 180 && ang < 270) {
            if(fabs(act->speed.x) > 5) {
                feet.y = brick_tmp->y + brick_tmp->brick_ref->image->h - (act->position.x-brick_tmp->x)*tan(act->angle);
                act->position.y = feet.y-diff;
            }
            else {
                act->angle = desired_angle;
                actor_move(act, v2d_new(0, -15*diff));
                act->is_jumping = TRUE;
            }
            friction = 1.5;
        }

        /* 270 wall */
        else if(ang == 270) {
            if(fabs(act->speed.x) > 5) {
                int myang = brick_downleft ? brick_downleft->brick_ref->angle : -1;
                if(brick_downleft && (myang > ang-90 && myang <= ang)) {
                    feet.y = brick_tmp->x + brick_tmp->brick_ref->image->w;
                    act->position.x = feet.y-diff;
                }
                else {
                    act->angle = desired_angle;
                    actor_move(act, v2d_new(-6.5*diff, 0));
                    act->is_jumping = TRUE;
                    act->speed = v2d_new(0, -0.7*fabs(act->speed.x));            
                }

            }
            else {
                act->angle = desired_angle;
                actor_move(act, v2d_new(-5*diff, 0));
                act->is_jumping = TRUE;
                act->ignore_horizontal = FALSE;
            }
            if(act->mirror & IF_HFLIP) friction = 1.5;
        }

        /* (270-360) slope */
        else if(ang > 270 && ang < 360) {
            feet.y = brick_tmp->y - (act->position.x-brick_tmp->x)*tan(act->angle);
            act->position.y = feet.y+diff;
            if(act->mirror & IF_HFLIP) friction = 0.2;
        }
    }




    /* x-axis */
    ds.x = (fabs(act->speed.x) > EPSILON) ? act->speed.x*dt + 0.5*((1.0-friction)*act->acceleration)*(dt*dt) : 0;
    if(input_button_down(act->input, IB_LEFT) && !input_button_down(act->input, IB_RIGHT)) {
        if(!act->ignore_horizontal && !input_button_down(act->input, IB_DOWN)) {
            act->speed.x = max(act->speed.x - (1.0-friction)*act->acceleration*dt, -act->maxspeed);
            act->mirror = IF_HFLIP;
        }
    }
    else if(input_button_down(act->input, IB_RIGHT) && !input_button_down(act->input, IB_LEFT)) {
        if(!act->ignore_horizontal && !input_button_down(act->input, IB_DOWN)) {
            act->speed.x = min(act->speed.x + (1.0-friction)*act->acceleration*dt, act->maxspeed);
            act->mirror = IF_NONE;
        }
    }
    else if(brick_down){
        int signal = 0;
        int ang = brick_down->brick_ref->angle;
        double factor;
        
        /* deceleration factor */
        factor = 1.0;

        /* deceleration */
        if(ang % 90 == 0) {
            if( (ang==90 && (act->mirror&IF_HFLIP) && act->speed.x < 0) || ((ang==270) && !(act->mirror&IF_HFLIP) && act->speed.x > 0) ) {
                if(act->speed.x > EPSILON) signal = 1;
                  else if(-act->speed.x > EPSILON) signal = -1;
                else signal = 0;
            }
            else {
                if(act->speed.x > EPSILON) signal = -1;
                  else if(-act->speed.x > EPSILON) signal = 1;
                else signal = 0;
            }
        }
        else if((ang > 90 && ang < 180) || (ang > 180 && ang < 270)){
            if(act->speed.x > EPSILON) signal = -1;
                 else if(-act->speed.x > EPSILON) signal = 1;
               else signal = 0;
        }

           act->speed.x += signal*factor*act->acceleration*dt;
    }


    /* done! */
       if(fabs(act->speed.x) < 1){ act->speed.x = ds.x = 0; }
    ds.x += level_brick_move_actor(brick_down,act).x*dt; /* moveable platforms II */
    return ds;
}



/*
 * actor_particle_movement()
 *
 * Basic particle movement. Returns a
 * delta_space vector.
 */
v2d_t actor_particle_movement(actor_t *act, double gravity)
{
    double dt = game_delta();
    v2d_t ds = v2d_new(0,0);

    /* x-axis */
    ds.x = act->speed.x*dt;

    /* y-axis */
    ds.y = act->speed.y*dt + 0.5*gravity*dt*dt;
    act->speed.y += gravity*dt;

    /* done! */
    return ds;
}




/*
 * actor_eightdirections_movement()
 *
 * Basic eight directions movement with acceleration.
 * Returns a delta_space vector.
 */
v2d_t actor_eightdirections_movement(actor_t *act)
{
    double dt = game_delta();
    v2d_t ds = v2d_new(0,0);

    /* input device */
    if(act->input) {
        /* x-speed */
        if(input_button_down(act->input, IB_RIGHT) && !input_button_down(act->input, IB_LEFT))
            act->speed.x = min(act->speed.x + act->acceleration*dt, act->maxspeed);
        if(input_button_down(act->input, IB_LEFT) && !input_button_down(act->input, IB_RIGHT))
            act->speed.x = max(act->speed.x - act->acceleration*dt, -act->maxspeed);
        if(!input_button_down(act->input, IB_LEFT) && !input_button_down(act->input, IB_RIGHT) && fabs(act->speed.x) > EPSILON) {
            if(act->speed.x > 0)
                act->speed.x = max(act->speed.x - act->acceleration*dt, 0);
            else
                act->speed.x = min(act->speed.x + act->acceleration*dt, 0);
        }

        /* y-speed */
        if(input_button_down(act->input, IB_DOWN) && !input_button_down(act->input, IB_UP))
            act->speed.y = min(act->speed.y + act->acceleration*dt, act->maxspeed);
        if(input_button_down(act->input, IB_UP) && !input_button_down(act->input, IB_DOWN))
            act->speed.y = max(act->speed.y - act->acceleration*dt, -act->maxspeed);
        if(!input_button_down(act->input, IB_UP) && !input_button_down(act->input, IB_DOWN) && fabs(act->speed.y) > EPSILON) {
            if(act->speed.y > 0)
                act->speed.y = max(act->speed.y - act->acceleration*dt, 0);
            else
                act->speed.y = min(act->speed.y + act->acceleration*dt, 0);
        }
    }
    else
        act->speed = v2d_new(0,0);

    /* done! */
    ds.x = fabs(act->speed.x) > EPSILON ? act->speed.x*dt + 0.5*act->acceleration*dt*dt : 0;
    ds.y = fabs(act->speed.y) > EPSILON ? act->speed.y*dt + 0.5*act->acceleration*dt*dt : 0;
    return ds;
}





/* private stuff */

/* brick_at(): given a list of bricks, returns
 * one that collides with the rectangle 'rect'
 * PS: this code ignores the bricks that are
 * not obstacles */
static brick_t* brick_at(brick_list_t *list, double rect[4])
{
    brick_t *ret = NULL;
    brick_list_t *p;
    int deg, inside_region, end = FALSE;
    double x, y, mytan, line;
    double br[4];

    /* main algorithm */
    for(p=list; p && !end; p=p->next) {

        if(p->data->brick_ref->property == BRK_NONE || !p->data->enabled)
            continue;

        br[0] = (double)p->data->x;
        br[1] = (double)p->data->y;
        br[2] = (double)(p->data->x + p->data->brick_ref->image->w);
        br[3] = (double)(p->data->y + p->data->brick_ref->image->h);

        if(bounding_box(rect, br)) {
            if(p->data->brick_ref->angle % 90 == 0) { /* if not slope */


                if(slope_priority) {
                    if(!ret) /* this code priorizes the slopes */
                        ret = p->data;
                    else {
                        if(floor_priority) {
                            if(ret->brick_ref->angle % 180 != 0) /* priorizes the floor/ceil */
                                ret = p->data;
                        }
                        else {
                            if(ret->brick_ref->angle % 180 == 0) /* priorizes the walls (not floor/ceil) */
                                ret = p->data;
                        }
                    }
                }
                else
                    ret = p->data; /* priorizes the floors & walls */


            }
            else if(slope_priority) { /* if slope */
                deg = p->data->brick_ref->angle;
                mytan = tan(deg * PI/180.0);
                for(x=rect[0]; x<=rect[2] && !end; x++) {
                    for(y=rect[1]; y<=rect[3] && !end; y++) {
                        inside_region = FALSE;

                        switch( (int)(deg/90) % 4 ) {
                            case 0: /* 1st quadrant */
                                line = br[3] + mytan*(br[0]-x);
                                inside_region = (br[0] <= x && x <= br[2] && line <= y && y <= br[3]);
                                break;
                            case 1: /* 2nd quadrant */
                                line = br[3] - mytan*(br[2]-x);
                                inside_region = (br[0] <= x && x <= br[2] && br[1] <= y && y <= line);
                                break;
                            case 2: /* 3rd quadrant */
                                line = br[3] - mytan*(br[0]-x);
                                inside_region = (br[0] <= x && x <= br[2] && br[1] <= y && y <= line);
                                break;
                            case 3: /* 4th quadrant */
                                line = br[3] + mytan*(br[2]-x);
                                inside_region = (br[0] <= x && x <= br[2] && line <= y && y <= br[3]);
                                break;
                        }

                        if(inside_region) {
                            ret = p->data;
                            end = TRUE;
                        }
                    }
                }
            }
        }
    }

    return ret;
}
